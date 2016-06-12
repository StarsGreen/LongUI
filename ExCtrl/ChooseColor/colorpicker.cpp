﻿#define _CRT_SECURE_NO_WARNINGS
#include "colorpicker.h"
#include <algorithm>
#include <LongUI.h>
#include <Control/UIColor.h>
#include "../UIColorHsv/UIColorHsv.h"
#include "../UIColorHsv/UIColorButton.h"
#include <../3rdParty/lz4/lib/lz4hc.h>
#include <../3rdParty/lz4/lib/lz4.h>
#undef min
#undef max
/*

    <HorizontalLayout weight="0.3">
        <ColorButton color="#6CF" colorpadding="8,8,8,8"
            borderwidth="1" margin="4,4,4,4" />
    </HorizontalLayout>
*/

// longui namespace
namespace LongUI {
    // layout
    const char* const COLOR_PCIKER_LAYOUT = 
        u8R"xml(<?xml version="1.0" encoding="utf-8"?>
<Window size="800, 600" titlename="LongUI HSV Color Picker" margin="8,8,8,8" >
    <HorizontalLayout>
        <ColorHSV name="hsvMain" h="sldH" s="sldS" v="sldV"
            r="sldR" g="sldG" b="sldB"/>
        <VerticalLayout borderwidth="1" >
            <HorizontalLayout size="0, 32">
                <Text text="H" size="32, 0"/>
                <Slider name="sldH" end="360" thumbsize="8,16" margin="4,4,4,4" />
                <Edit name="edtH" text="0.00" margin="4,4,4,4" borderwidth="1" size="64, 0"/>
                <Text name="txtH" text="0" size="32, 0"/>
            </HorizontalLayout>
            <HorizontalLayout size="0, 32">
                <Text text="S" size="32, 0"/>
                <Slider name="sldS" value="1" thumbsize="8,16" margin="4,4,4,4" />
                <Edit name="edtS" text="1.00" margin="4,4,4,4" borderwidth="1" size="64, 0"/>
                <Text name="txtS" text="100" size="32, 0"/>
            </HorizontalLayout>
            <HorizontalLayout size="0, 32">
                <Text text="V" size="32, 0"/>
                <Slider name="sldV" value="1" thumbsize="8,16" margin="4,4,4,4" />
                <Edit name="edtV" text="1.00" margin="4,4,4,4" borderwidth="1" size="64, 0"/>
                <Text name="txtV" text="100" size="32, 0"/>
            </HorizontalLayout>
            <HorizontalLayout size="0, 32">
                <Text text="R" size="32, 0"/>
                <Slider name="sldR" value="1" thumbsize="8,16" margin="4,4,4,4" />
                <Edit name="edtR" text="1.00" margin="4,4,4,4" borderwidth="1" size="64, 0"/>
                <Text name="txtR" text="255" size="32, 0"/>
            </HorizontalLayout>
            <HorizontalLayout size="0, 32">
                <Text text="G" size="32, 0"/>
                <Slider name="sldG" value="0" thumbsize="8,16" margin="4,4,4,4" />
                <Edit name="edtG" text="0.00" margin="4,4,4,4" borderwidth="1" size="64, 0"/>
                <Text name="txtG" text="0" size="32, 0"/>
            </HorizontalLayout>
            <HorizontalLayout size="0, 32">
                <Text text="B" size="32, 0"/>
                <Slider name="sldB" value="0" thumbsize="8,16" margin="4,4,4,4" />
                <Edit name="edtB" text="0.00" margin="4,4,4,4" borderwidth="1" size="64, 0"/>
                <Text name="txtB" text=  "0" size="32, 0"/>
            </HorizontalLayout>
            <HorizontalLayout size="0, 32">
                <Text text="A" size="32, 0"/>
                <Slider name="sldA" value="1" thumbsize="8,16" margin="4,4,4,4" />
                <Edit name="edtA" text="1.00" margin="4,4,4,4" borderwidth="1" size="64, 0"/>
                <Text name="txtA" text="255" size="32, 0"/>
            </HorizontalLayout>
            <Null/>
            <HorizontalLayout size="0, 32">
                <Text name="txtHtml" margin="4,4,4,4" text="HTML Format:" size="-1, 0"/>
                <Edit name="edtHtml" text="#FF0000" margin="4,4,4,4" borderwidth="1"/>
            </HorizontalLayout>
        </VerticalLayout>
    </HorizontalLayout>
    <HorizontalLayout size="0, 64">
        <VerticalLayout size="100, 0">
            <Text  name="txtNow"  text="Current"/>
            <Text  name="txtOld"  text="Previous"/>
        </VerticalLayout>
        <VerticalLayout weight="2">
            <Color name="colNowA" color="red"   direct="true"/>
            <Color name="colOldA" color="black" direct="true"/>
        </VerticalLayout>
        <VerticalLayout weight="2">
            <Color name="colNowB" color="red"/>
            <Color name="colOldB" color="black"/>
        </VerticalLayout>
        <VerticalLayout>
            <Button text="▶" name="btnDo" borderwidth="1" margin="4,4,4,4"/>
            <Null/>
        </VerticalLayout>
        <VerticalLayout>
            <ColorButton name="cbn1" borderwidth="1" margin="4,4,4,4"/>
            <ColorButton name="cbn2" borderwidth="1" margin="4,4,4,4"/>
        </VerticalLayout>
        <VerticalLayout>
            <ColorButton name="cbn3" borderwidth="1" margin="4,4,4,4"/>
            <ColorButton name="cbn4" borderwidth="1" margin="4,4,4,4"/>
        </VerticalLayout>
        <VerticalLayout>
            <ColorButton name="cbn5" borderwidth="1" margin="4,4,4,4"/>
            <ColorButton name="cbn6" borderwidth="1" margin="4,4,4,4"/>
        </VerticalLayout>
        <VerticalLayout>
            <ColorButton name="cbn7" borderwidth="1" margin="4,4,4,4"/>
            <ColorButton name="cbn8" borderwidth="1" margin="4,4,4,4"/>
        </VerticalLayout>
        <VerticalLayout>
            <ColorButton name="cbn9" borderwidth="1" margin="4,4,4,4"/>
            <ColorButton name="cbna" borderwidth="1" margin="4,4,4,4"/>
        </VerticalLayout>
        <VerticalLayout>
            <ColorButton name="cbnb" borderwidth="1" margin="4,4,4,4"/>
            <ColorButton name="cbnc" borderwidth="1" margin="4,4,4,4"/>
        </VerticalLayout>
    </HorizontalLayout>
    <HorizontalLayout size="0, 32">
        <Null/>
        <Button name="btnOK" size="-1, 0" margin="4,4,4,4" borderwidth="1" text="OK"/>
        <Button name="btnCl" size="-1, 0" margin="4,4,4,4" borderwidth="1" text="Cancel"/>
    </HorizontalLayout>
</Window>
)xml";
}

// longui::impl 命名空间
namespace LongUI { namespace impl { 
    // to longui
    inline auto&longui(const D2D1_COLOR_F& c) noexcept {
        return reinterpret_cast<const ColorPickerF&>(c);
    }
    // to d2d
    inline auto&d2d(const ColorPickerF& c) noexcept {
        return reinterpret_cast<const D2D1_COLOR_F&>(c);
    }
    // clamp 01
    void clamp01(float& f) noexcept {
        f = std::max(std::min(f, 1.f), 0.f);
    }
    // is legal color

}}


/// <summary>
/// Initializes a new instance of the <see cref="CUIColorPicker"/> class.
/// </summary>
LongUI::CUIColorPicker::CUIColorPicker() noexcept {
    ColorPickerF dc { 1.f, 1.f, 1.f, 1.f };
    for (auto&c : m_aSaveColor) c = dc;
    std::memset(m_pTexted, 0, sizeof(m_pTexted));
    std::memset(m_apSaveColor, 0, sizeof(m_apSaveColor));
}

/// <summary>
/// Finalizes an instance of the <see cref="CUIColorPicker"/> class.
/// </summary>
/// <returns></returns>
LongUI::CUIColorPicker::~CUIColorPicker() noexcept {

}

/// <summary>
/// Sets the old color.
/// </summary>
/// <param name="c">The c.</param>
/// <returns></returns>
void LongUI::CUIColorPicker::SetOldColor(const D2D1_COLOR_F& c) noexcept {
    m_colorOld = impl::longui(c);
    this->set_old_color(c);
}

/// <summary>
/// Sets the old color.
/// </summary>
/// <param name="c">The c.</param>
/// <returns></returns>
void LongUI::CUIColorPicker::set_old_color(const D2D1_COLOR_F& c) noexcept {
    // 存在窗口
    if (!m_pWindow) return;
    auto& window = *m_pWindow;
    // 设置
    auto set = [&](const char* name) noexcept {
        auto a = window.FindControl(name);
        auto b = longui_cast<UIColor*>(a);
        b->SetColor(c);
    };
    // 指定控件
    set("colOldA"); set("colOldB");
}

// longui namespace
namespace LongUI {
    // viewport for color picker
    class UIPickerView final : public UIViewport {
        // super class
        using Super = UIViewport;
        // clean up
        void cleanup() noexcept override { before_deleted(); delete this; }
    public:
        // ctor
        UIPickerView(XUIBaseWindow* window) : Super(window) {}
        // do some event
        bool DoEvent(const EventArgument& arg) noexcept override {
            // longui event
            switch (arg.event)
            {
            case LongUI::Event::Event_SubEvent:
                this->subevent(arg.ui.subevent, arg.sender);
                return true;
            case LongUI::Event::Event_TreeBuildingFinished:
                this->tree_bulit();
                __fallthrough;
            default:
                return Super::DoEvent(arg);
            }
        }
    private:
        // tree finished
        void tree_bulit() noexcept;
        // tree finished
        void subevent(SubEvent e, UIControl* sender) noexcept {
            if (e == LongUI::SubEvent::Event_ItemClicked) {
            }
        }
    public:
        // picker
        CUIColorPicker*         picker = nullptr;
    };
}

/// <summary>
/// Creates the specified parent.
/// </summary>
/// <param name="parent">The parent.</param>
/// <returns></returns>
bool LongUI::CUIColorPicker::Create(XUIBaseWindow* parent) noexcept {
    assert(m_pWindow == nullptr);
    bool a = UIManager.IsRegisteredControlClass("ColorHSV");
    bool b = UIManager.IsRegisteredControlClass("ColorButton");
#ifdef _DEBUG
    /* 注册例子 {
        cc = LongUI::UIColorHsv::CreateControl;
        UIManager.RegisterControlClass(cc, "ColorHSV");
        cc = LongUI::UIColorButton::CreateControl;
        UIManager.RegisterControlClass(cc, "ColorButton");
    }*/
    assert(a && b && "you must register the control!");
#endif
    if(!(a && b)) return false;
#if 0
    {
        auto laylen = int(std::strlen(COLOR_PCIKER_LAYOUT));
        auto buflen = ::LZ4_compressBound(laylen);
        LongUI::SafeBuffer<char>(size_t(buflen), [=](char* buf) noexcept {
            auto src = COLOR_PCIKER_LAYOUT;
            auto code = ::LZ4_compress_HC(src, buf, laylen, buflen, 16);
            assert(code > 0);
            if (auto file = std::fopen("xml.txt", "w")) {
                std::for_each(buf, buf + code, [=](char ch) noexcept {
                    std::fprintf(file, "\\x%x", int(unsigned char(ch)));
                });
                std::fclose(file);
            }
            /*if (auto out = std::malloc(16 * 1024)) {
                auto outchar = reinterpret_cast<char*>(out);
                auto dcode = ::LZ4_decompress_fast(buf, outchar, laylen);
                outchar[laylen] = 0;
                std::free(out);
            }*/
        });
    }
#endif
    // 创建窗口
    m_pWindow = UIManager.CreateUIWindow<UIPickerView>(COLOR_PCIKER_LAYOUT);
    // 成功时写入指针
    if (m_pWindow) {
        auto view = m_pWindow->GetViewport();
        static_cast<UIPickerView*>(view)->picker = this;
    }
    // 正式创建
    return !!m_pWindow;
}

/// <summary>
/// Sets the name of the title.
/// </summary>
/// <param name="str">The string for title name.</param>
/// <returns></returns>
void LongUI::CUIColorPicker::SetTitleName(const wchar_t* str) noexcept {
    assert(m_pWindow && "check return code for CUIColorPicker::Create");
    assert(str && "bad argument");
    m_pWindow->SetTitleName(str);
}

/// <summary>
/// Shows the window.
/// </summary>
/// <param name="nCmdShow">The n command show.</param>
/// <returns></returns>
void LongUI::CUIColorPicker::ShowWindow(int nCmdShow) noexcept {
    assert(m_pWindow && "check return code for CUIColorPicker::Create");
    m_pWindow->ShowWindow(nCmdShow);
}

/// <summary>
/// Closes the window.
/// </summary>
/// <returns></returns>
void LongUI::CUIColorPicker::CloseWindow() noexcept {
    assert(m_pWindow && "check return code for CUIColorPicker::Create");
    m_pWindow->Close();
}

/// <summary>
/// Sets the text.
/// </summary>
/// <param name="index">The index.</param>
/// <param name="text">The text.</param>
/// <returns></returns>
void LongUI::CUIColorPicker::SetText(
    ControlIndex index, const wchar_t * text) noexcept {
}

/// <summary>
/// Trees bulit.
/// </summary>
/// <returns></returns>
void LongUI::UIPickerView::tree_bulit() noexcept {
    auto window = m_pWindow;
    auto hsv = longui_cast<UIColorHsv*>(window->FindControl("hsvMain"));
    assert(hsv && "control not found");
    // hsv
    {
        const auto a = window->FindControl("colNowA");
        const auto b = window->FindControl("colNowB");
        const auto c = window->FindControl("sldA");
        const auto html = window->FindControl("edtHtml");
        assert(a && b && c && "control not found");
        assert(html && "control not found");
        {
            const auto ca = longui_cast<UIColor*>(a);
            const auto cb = longui_cast<UIColor*>(b);
            const auto color_changed = [=](UIControl*) noexcept {
                auto base = hsv->PickColor();
                base.a = c->GetFloat();
                ca->SetColor(base);
                cb->SetColor(base);
                return true;
            };
            hsv->Add_OnChanged(color_changed);
            c->Add_OnChanged(color_changed);
            const auto fa = window->FindControl("edtA");
            const auto ia = window->FindControl("txtA");
            c->Add_OnChanged([fa, ia](UIControl* alpha) noexcept {
                CUIString str;
                float a = alpha->GetFloat();
                str.Format(L"%.3f", a); fa->SetText(str);
                str.Format(L"%d", int(a*255.f)); ia->SetText(str);
                return true;
            });
            hsv->Add_OnChanged([=](UIControl* ) noexcept {
                CUIString str;
                auto base = hsv->PickColor();
                str.Format(
                    L"#%02X%02X%02X",
                    int(base.r*255.f + 0.5f),
                    int(base.g*255.f + 0.5f),
                    int(base.b*255.f + 0.5f)
                );
                // 不同再修改
                html->SetText(str);
                return true;
            });
            // 修改HTML文本
            html->Add_OnChanged([=](UIControl* h) noexcept {
                char buf[32];
                auto str = h->GetText();
                auto len = std::wcslen(str);
                // 缓存不足
                if (len + 1 < lengthof(buf)) {
                    auto itr = buf;
                    for (; *str; ++itr, ++str) *itr = char(*str);
                    *itr = 0;
                    D2D1_COLOR_F color { 1.f, 1.f, 1.f, 1.f };
                    Helper::MakeColor(buf, color);
                    impl::clamp01(color.r);
                    impl::clamp01(color.g);
                    impl::clamp01(color.b);
                    impl::clamp01(color.a);
                    hsv->SetColor(color);
                }
                return true;
            });
        }
    }
    // RGB
    {
        const auto fr = window->FindControl("edtR");
        const auto fg = window->FindControl("edtG");
        const auto fb = window->FindControl("edtB");
        const auto ir = window->FindControl("txtR");
        const auto ig = window->FindControl("txtG");
        const auto ib = window->FindControl("txtB");
        const auto fh = window->FindControl("edtH");
        const auto fs = window->FindControl("edtS");
        const auto fv = window->FindControl("edtV");
        const auto ih = window->FindControl("txtH");
        const auto is = window->FindControl("txtS");
        const auto iv = window->FindControl("txtV");
        assert(fr && fg && fb && "control not found");
        assert(ir && ig && ib && "control not found");
        assert(fh && fs && fv && "control not found");
        assert(ih && is && iv && "control not found");
        // 修改事件
        hsv->Add_OnChanged([=](UIControl*) noexcept {
            CUIString str;
            auto base = hsv->PickColor();
            auto hsv_h = hsv->GetH();
            auto hsv_s = hsv->GetS();
            auto hsv_v = hsv->GetV();
            str.Format(L"%.1f", hsv_h ); fh->SetText(str);
            str.Format(L"%d", int(hsv_h)); ih->SetText(str);

            str.Format(L"%.3f", hsv_s ); fs->SetText(str);
            str.Format(L"%d", int(hsv_s*100.f)); is->SetText(str);

            str.Format(L"%.3f", hsv_v ); fv->SetText(str);
            str.Format(L"%d", int(hsv_v*100.f)); iv->SetText(str);

            str.Format(L"%.3f", base.r); fr->SetText(str);
            str.Format(L"%d", int(base.r*255.f)); ir->SetText(str);

            str.Format(L"%.3f", base.g); fg->SetText(str);
            str.Format(L"%d", int(base.g*255.f)); ig->SetText(str);

            str.Format(L"%.3f", base.b); fb->SetText(str);
            str.Format(L"%d", int(base.b*255.f)); ib->SetText(str);

            return true;
        });
    }
    // H
    {
        const auto slider = window->FindControl("sldH");
        const auto edit = window->FindControl("edtH");
        assert(slider && edit);
        auto on_changed = [=](UIControl* hc) noexcept {
            auto h = std::abs(hc->GetFloat());
            const float end = 359.99f;
            hsv->SetH(h >= 360.f ? end : h);
            return true;
        };
        slider->Add_OnChanged(on_changed);
        edit->Add_OnChanged(on_changed);
    }
    // S
    {
        const auto slider = window->FindControl("sldS");
        const auto edit = window->FindControl("edtS");
        assert(slider && edit);
        auto on_changed = [=](UIControl* sc) noexcept {
            auto s = sc->GetFloat();
            hsv->SetS(std::max(std::min(s, 1.f), 0.f));
            return true;
        };
        slider->Add_OnChanged(on_changed);
        edit->Add_OnChanged(on_changed);
    }
    // V
    {
        const auto slider = window->FindControl("sldV");
        const auto edit = window->FindControl("edtV");
        assert(slider && edit);
        auto on_changed = [=](UIControl* vc) noexcept {
            auto v = vc->GetFloat();
            hsv->SetV(std::max(std::min(v, 1.f), 0.f));
            return true;
        };
        slider->Add_OnChanged(on_changed);
        edit->Add_OnChanged(on_changed);
    }
    // R
    {
        const auto slider = window->FindControl("sldR");
        const auto edit = window->FindControl("edtR");
        assert(slider && edit);
        auto on_changed = [=](UIControl* rc) noexcept {
            auto r = rc->GetFloat();
            auto color = hsv->PickColor();
            color.r = std::max(std::min(r, 1.f), 0.f);
            hsv->SetColor(color);
            return true;
        };
        slider->Add_OnChanged(on_changed);
        edit->Add_OnChanged(on_changed);
    }
    // G
    {
        const auto slider = window->FindControl("sldG");
        const auto edit = window->FindControl("edtG");
        assert(slider && edit);
        auto on_changed = [=](UIControl* gc) noexcept {
            auto g = gc->GetFloat();
            auto color = hsv->PickColor();
            color.g = std::max(std::min(g, 1.f), 0.f);
            hsv->SetColor(color);
            return true;
        };
        slider->Add_OnChanged(on_changed);
        edit->Add_OnChanged(on_changed);
    }
    // B
    {
        const auto slider = window->FindControl("sldB");
        const auto edit = window->FindControl("edtB");
        assert(slider && edit);
        auto on_changed = [=](UIControl* bc) noexcept {
            auto b = bc->GetFloat();
            auto color = hsv->PickColor();
            color.b = std::max(std::min(b, 1.f), 0.f);
            hsv->SetColor(color);
            return true;
        };
        slider->Add_OnChanged(on_changed);
        edit->Add_OnChanged(on_changed);
    }
}