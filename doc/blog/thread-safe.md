### 线程安全

LongUI默认(应该)拥有两条线程: UI线程与渲染线程.

UIControl::Update和Render总是在渲染线程中调用, 这也是设计 UIControl::Update的原因: 在渲染前一次性计算需要渲染数据, 然后渲染. 同时也是UIControl::Render是const的原因: **别™改了**

LongUI拥有两把锁, 数据锁与渲染锁。渲染线程先进行数据刷新, 然后进行数据渲染:

```c
void one_frame() {
    data_lock();
    data_update();
    data_unlock();
    
    render_lock();
    render();
    render_unlock();
}
```

一般情况如果修改基本数据, 就应该添加数据锁; 修改渲染数据就需要渲染锁. 但是LongUI强烈建议渲染数据应该被隐藏起来: **在data_update时检测渲染数据的副本被修改了, 然后向渲染器提交新的渲染数据**. 这时候只需要无脑地使用数据锁而不用担心渲染锁(除非必须及时修改渲染数据, 窗口级一般会需要)

### 时间胶囊 Time Capsule
时间胶囊是LongUI异步处理的一种办法: **将函数包装放在未来一段时间每帧连续调用直到时间耗尽**. 当然, 这个类一开始设计的时候是设计成放在未来某时刻调用一次, 所以取名为时间胶囊. 后来发现有些东西需要连续调用干脆就合并在里面了, 也**懒**得换名字.

时间胶囊总是在渲染时, **在UI线程**进行调用(默认双线程情况):

```c
void one_frame() {
    data_lock();
    data_update();
    data_unlock();
    
    signal_time_capsule();
    
    render_lock();
    render();
    render_unlock();

    wait_for_time_capsule();
}
```

可以看出时间胶囊总是**在渲染时处理**, 也就是说在时间胶囊中的函数无需使用数据锁. 但是也不建议使用渲染锁, 因为会强制等待. 虽然是在UI线程, 但是不允许调用堵塞(blocking)函数, 会导致渲染线程一直在``` wait_for_time_capsule() ```里面等待

为了线程安全, LongUI强烈建议渲染数据应该被隐藏起来(标记被修改了然后等待下帧data_update处理).可以参考UILabel::SetText的实现(经过简化):

```cpp

class UILabel {
    // ..........

    // m_text是属于渲染数据, 渲染时其他地方修改会造成线程安全隐患
    CUITextLayout           m_text;
    // m_string就是普通的字符串, 渲染时本身无用所以可以在这时其他线程安全修改
    CUIString               m_string;

    void SetText(CUIString&& s) {
        // ......
        m_bTextChanged = true;
    }

    void Update() override {
        // ......
        if (m_bTextChanged) {
            m_bTextChanged = false;
            this->on_text_changed();
        }
        // ......
    }
    
    void Render() const override {
        // ......
        m_text.Render();
    }
};

```

换句话说时间胶囊中的函数应该是无锁编程!

#### 利用CUIBlockingGuiOpAutoUnlocker调用堵塞函数

LongUI仅仅是一个简单的GUI库, 不是一套完整的解决方案, 不可能像Qt那样封装全部操作系统的函数. 有时可能需要调用一些操作系统GUI函数: 最简单的比如``` MessageBox ```, 稍微复杂点的 ``` GetOpenFileName ``` , 这些都是堵塞型的.

默认情况下为了安全, 所有的GUI操作都是加了锁的. 比如点击按钮弹出``` MessageBox ```, 点击时上了数据锁, 导致整个msgbox都在数据上锁的状态, 所以为了渲染线程能够安全运行就封装了``` CUIBlockingGuiOpAutoUnlocker ```

大概应该这么用:
```cpp
void call() {
    // ......
    int rv_msgbox = 0;
    {
        CUIBlockingGuiOpAutoUnlocker unlocker;
        const auto text = L"ASDDSA";
        const auto capt = L"QWEEWQ";
        rv_msgbox = ::MessageBoxW(window->GetHwnd(), text, capt, MB_YESNO);
    }
    // ......
}
```
或者用上匿名表达式减少``` rv_msgbox ```中间状态(const anywhere):
```cpp
void call() {
    // ......
    const auto rv_msgbox = [window]() {
        CUIBlockingGuiOpAutoUnlocker unlocker;
        const auto text = L"ASDDSA";
        const auto capt = L"QWEEWQ";
        return ::MessageBoxW(window->GetHwnd(), text, capt, MB_YESNO);
    }();
    // ......
}
```

#### 利用 UIControl::ControlMakingBegin/End 创建大量控件
为了保证正确性, 请在大量创建控件时使用``` UIControl::ControlMakingBegin ``` 和 ``` UIControl::ControlMakingEnd ```包裹创建过程, 比如``` UIControl::SetXul ```是这样实现的:
```cpp
void LongUI::UIControl::SetXul(const char* xul) noexcept {
    UIControl::ControlMakingBegin();
    CUIControlControl::MakeXul(*this, xul);
    UIControl::ControlMakingEnd();
}
```
这样做是为了保证这些控件是"同时"创建的, 否则可能前面的控件是上一帧创建, 后面的控件是后一帧创建.