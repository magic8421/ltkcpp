//////////////////////////////////////////////////////////////////////////
// Author:    Sara Chen
// Email:     6659907@163.com
// QQ:        314266265
// License:   MIT license
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
#include "ltk.h"
#include "atlstr.h"
#include "Window.h"
#include "ShadowFrame.h"
#include "StyleManager.h"
#include "RefCounted.h"
#include "Sprite.h"
#include "TimerManager.h"
#include "SetupStyles.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

static ID2D1Factory *g_d2d_factory = NULL;
static IWICImagingFactory  *g_wic_factory = NULL;
static IDWriteFactory *g_dw_factory = NULL;
static ULONG_PTR m_gdiplusToken;

namespace ltk {
    ID2D1Factory *GetD2DFactory() { return g_d2d_factory; }
    IWICImagingFactory *GetWICFactory() { return g_wic_factory; }
    IDWriteFactory *GetDWriteFactory() { return g_dw_factory; }

    // convert DIP to screen
    void DipCoordToScreen(float &x, float &y) 
    {
		#pragma warning(push)
		#pragma warning(disable:4996)
		#pragma warning(disable:28159)
        float dpi_x = 0.0f;
        float dpi_y = 0.0f;
        g_d2d_factory->GetDesktopDpi(&dpi_x, &dpi_y); // wtf? non-square pixel?

        x = dpi_x * x / 96.0f;
        y = dpi_y * y / 96.0f;
		#pragma warning(pop)
    }

    // convert screen to DIP
    void ScreenCoordToDip(float &x, float &y)
    {
		#pragma warning(push)
		#pragma warning(disable:4996)
		#pragma warning(disable:28159)

        static float dpi_x = 0.0f;
        static float dpi_y = 0.0f;

		if (dpi_x == 0.0f) {
			g_d2d_factory->GetDesktopDpi(&dpi_x, &dpi_y); // wtf? non-square pixel?
		}
        x = x * 96.0f / dpi_x;
        y = y * 96.0f / dpi_y;

		#pragma warning(pop)
    }

    RECT DipRectToScreen(const RectF &rc)
    {
		#pragma warning(push)
		#pragma warning(disable: 4244)
		#pragma warning(disable:28159)
		#pragma warning(disable:4996)
		float dpi_x = 0.0f;
        float dpi_y = 0.0f;
        g_d2d_factory->GetDesktopDpi(&dpi_x, &dpi_y); // wtf? non-square pixel?

        RECT out;

        out.left = dpi_x * rc.X / 96.0f;
        out.top = dpi_y * rc.Y / 96.0f;
        out.right = out.left + dpi_x * rc.Width / 96.0f;
        out.bottom = out.top + dpi_y * rc.Height / 96.0f;
        #pragma warning(pop)
        return out;
    }

    // try to snap to screen pixel
    static void AdjustRect(D2D1_RECT_F &src, D2D1_RECT_F &dst, float scale)
    {
        //src.left = (float)(int)(src.left + 0.5f);
        //src.top = (float)(int)(src.top + 0.5f);
        //src.right = (float)(int)(src.right + 0.5f);
        //src.bottom = (float)(int)(src.bottom + 0.5f);

        //MapCoordByDpi(dst.left, dst.top);
        //MapCoordByDpi(dst.right, dst.bottom);
        //dst.left = (float)(int)(dst.left + 2.5f);
        //dst.top = (float)(int)(dst.top + 2.5f);
        //dst.right = (float)(int)(dst.right + 2.5f);
        //dst.bottom = (float)(int)(dst.bottom + 2.5f);
        //UnmapCoordByDpi(dst.left, dst.top);
        //UnmapCoordByDpi(dst.right, dst.bottom);
    }

    void DrawTextureNineInOne(ID2D1RenderTarget *target, ID2D1Bitmap *bitmap, 
        const RectF &atlas, const Margin& margin, RectF dst2, float opacity, float)
    {
        D2D1_BITMAP_INTERPOLATION_MODE interp_mode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
        D2D1_RECT_F src;
        D2D1_RECT_F dst;
        Margin dMargin;
        float scale = 1.0f;
        float scale2 = 1.0f;

        ScreenCoordToDip(scale, scale2);
        dMargin.left = margin.left * scale;
        dMargin.top = margin.top * scale;
        dMargin.right = margin.right * scale;
        dMargin.bottom = margin.bottom * scale;

        // left top
        src.left = atlas.X;
        src.top = atlas.Y;
        src.right = atlas.X + margin.left;
        src.bottom = atlas.Y + margin.top;
        dst.left = dst2.X;
        dst.top = dst2.Y;
        dst.right = dst2.X + dMargin.left;
        dst.bottom = dst2.Y + dMargin.top;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);

        // left center
        src.left = atlas.X;
        src.top = atlas.Y + margin.top;
        src.right = atlas.X + margin.left;
        src.bottom = atlas.Y + atlas.Height - margin.bottom;
        dst.left = dst2.X;
        dst.top = dst2.Y + dMargin.top ;
        dst.right = dst2.X + dMargin.left;
        dst.bottom = dst2.Y + dst2.Height - dMargin.bottom;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);
    
        // left bottom
        src.left = atlas.X;
        src.top = atlas.Y + atlas.Height - margin.bottom;
        src.right = atlas.X + margin.left;
        src.bottom = atlas.Y + atlas.Height;
        dst.left = dst2.X;
        dst.top = dst2.Y + dst2.Height - dMargin.bottom;
        dst.right = dst2.X + dMargin.left;
        dst.bottom = dst2.Y + dst2.Height;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);

        // center top
        src.left = atlas.X + margin.left;
        src.top = atlas.Y;
        src.right = atlas.X + atlas.Width - margin.right;
        src.bottom = atlas.Y + margin.top;
        dst.left = dst2.X + dMargin.left;
        dst.top = dst2.Y;
        dst.right = dst2.X + dst2.Width - dMargin.right;
        dst.bottom = dst2.Y + dMargin.top;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);

        // center center
        src.left = atlas.X + margin.left;
        src.top = atlas.Y + margin.top;
        src.right = atlas.X + atlas.Width - margin.right;
        src.bottom = atlas.Y + atlas.Height - margin.bottom;
        dst.left = dst2.X + dMargin.left;
        dst.top = dst2.Y + dMargin.top;
        dst.right = dst2.X + dst2.Width - dMargin.right;
        dst.bottom = dst2.Y + dst2.Height - dMargin.bottom;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);

        // center bottom
        src.left = atlas.X + margin.left;
        src.top = atlas.Y + atlas.Height - margin.bottom;
        src.right = atlas.X + atlas.Width - margin.right;
        src.bottom = atlas.Y + atlas.Height;
        dst.left = dst2.X + dMargin.left;
        dst.top = dst2.Y + dst2.Height - dMargin.bottom;
        dst.right = dst2.X + dst2.Width - dMargin.right;
        dst.bottom = dst2.Y + dst2.Height;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);

        // right top
        src.left = atlas.X + atlas.Width - margin.right;
        src.top = atlas.Y;
        src.right = atlas.X + atlas.Width;
        src.bottom = atlas.Y + margin.top;
        dst.left = dst2.X + dst2.Width - dMargin.right;
        dst.top = dst2.Y;
        dst.right = dst2.X + dst2.Width;
        dst.bottom = dst2.Y + dMargin.top;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);

        // right center
        src.left = atlas.X + atlas.Width - margin.right;
        src.top = atlas.Y + margin.top;
        src.right = atlas.X + atlas.Width;
        src.bottom = atlas.Y + atlas.Height - margin.bottom;
        dst.left = dst2.X + dst2.Width - dMargin.right;
        dst.top = dst2.Y + dMargin.top;
        dst.right = dst2.X + dst2.Width;
        dst.bottom = dst2.Y + dst2.Height - dMargin.bottom;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);

        // right bottom
        src.left = atlas.X + atlas.Width - margin.right;
        src.top = atlas.Y + atlas.Height - margin.bottom;
        src.right = atlas.X + atlas.Width;
        src.bottom = atlas.Y + atlas.Height;
        dst.left = dst2.X + dst2.Width - dMargin.right;
        dst.top = dst2.Y + dst2.Height - dMargin.bottom;
        dst.right = dst2.X + dst2.Width;
        dst.bottom = dst2.Y + dst2.Height;
        AdjustRect(src, dst, scale);
        target->DrawBitmap(bitmap, dst, opacity, interp_mode, src);
    }

	void DrawTextureThreePatchHorizontal(ID2D1RenderTarget *target, ID2D1Bitmap *bitmap,
		const RectF &atlas, const Margin& margin, RectF dst2, float opacity, float scale)
	{}

    void DrawRectSnapped(ID2D1RenderTarget *target, const RectF &rc, ID2D1Brush *brush)
    {
        D2D1_RECT_F rc2 = D2D1RectF(rc);
        DipCoordToScreen(rc2.left, rc2.top);
        DipCoordToScreen(rc2.right, rc2.bottom);
        rc2.left = Round45(rc2.left) + 0.5f;
        rc2.top = Round45(rc2.top) + 0.5f;
        rc2.right = Round45(rc2.right) + 0.5f;
        rc2.bottom = Round45(rc2.bottom) + 0.5f;
        ScreenCoordToDip(rc2.left, rc2.top);
        ScreenCoordToDip(rc2.right, rc2.bottom);

        static float dpi_x = 0.0f;
        static float dpi_y = 0.0f;
		#pragma warning(push)
		#pragma warning(disable:28159)
		#pragma warning(disable:4996)
		if (dpi_x == 0.0f) {
            g_d2d_factory->GetDesktopDpi(&dpi_x, &dpi_y); // wtf? non-square pixel?
        }
		#pragma warning(pop)
        target->DrawRectangle(rc2, brush, 96.0f / dpi_x);
    }

    void TranslateTransform(ID2D1RenderTarget *target, float dx, float dy)
    {
        D2D1_MATRIX_3X2_F matrix;
        target->GetTransform(&matrix);
        matrix._31 += dx;
        matrix._32 += dy;
        target->SetTransform(&matrix);
    }




	HRESULT LoadBitmapFromFile(ID2D1RenderTarget *target, LPCWSTR path, ID2D1Bitmap **bitmap)
	{
		IWICBitmapDecoder *decorder = NULL;
		IWICBitmapFrameDecode *frame = NULL;
		IWICFormatConverter *converter = NULL;
		assert(*bitmap == nullptr);

		HRESULT hr = g_wic_factory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decorder);
		if (SUCCEEDED(hr))
		{
			hr = decorder->GetFrame(0, &frame);
			assert(SUCCEEDED(hr));
			hr = g_wic_factory->CreateFormatConverter(&converter);
			assert(SUCCEEDED(hr));
			hr = converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
				NULL, 0.0, WICBitmapPaletteTypeMedianCut);
			assert(SUCCEEDED(hr));
			hr = target->CreateBitmapFromWicBitmap(converter, NULL, bitmap);
			assert(SUCCEEDED(hr));
			SAFE_RELEASE(converter);
			SAFE_RELEASE(frame);
			SAFE_RELEASE(decorder);
			return S_OK;
		}
		return E_FAIL;
	}

	DWORD TickCount()
	{
#pragma warning(push)
#pragma warning(disable:28159)
		return ::GetTickCount();
#pragma warning(pop)
	}

	void LtkInitialize()
    {
        ::CoInitialize(NULL);

        if (g_d2d_factory) {
            LTK_LOG("already initialized.");
            return;
        }

        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_d2d_factory);
        assert(SUCCEEDED(hr));

        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory,
            (LPVOID*)&g_wic_factory
            );
        assert(SUCCEEDED(hr));

        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&g_dw_factory)
            );
        assert(SUCCEEDED(hr));

        // InitInstance
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

        LtkLogInit();
        TimerManager::Instance();
        ShadowFrame::Init();
        Window::RegisterWndClass();

		StyleManager::NewTheme("rect");
		SetupVectorStyleDark();

		StyleManager::NewTheme("pixel");
		SetupPixelStyle1();

		LTK_LOG("sizeof map: %d", sizeof(std::map<int, void*>));
    }

    void LtkUninitialize()
    {
		ShadowFrame::Free();
        StyleManager::Free();
        TimerManager::Free();

        g_dw_factory->Release();
        g_wic_factory->Release();
        g_d2d_factory->Release();

        Gdiplus::GdiplusShutdown(m_gdiplusToken);

        ::CoUninitialize();

		Object::DumpObjectLeaks();
	}

    IconInfo& IconInfo::SetAtlas(float x, float y, float w, float h)
    {
        this->atlas = RectF(x, y, w, h);
        return *this;
    }

    IconInfo& IconInfo::SetScale(float s)
    {
        this->scale = s;
        return *this;
    }

    TextureInfo& TextureInfo::SetAtals(float x, float y, float w, float h)
    {
        this->atlas = RectF(x, y, w, h);
        return *this;
    }

    TextureInfo& TextureInfo::SetMargin(float left, float top, float right, float bottom)
    {
        this->margin.left = left;
        this->margin.top = top;
        this->margin.right = right;
        this->margin.bottom = bottom;
        return *this;
    }

}

using namespace ltk;

/*
void test_gdip_font()
{
    Gdiplus::Font *font = nullptr;
    for (size_t i = 0; i < 1000000; i++)
    {
        font = new Gdiplus::Font(L"Î¢ÈíÑÅºÚ", 16);
    }
}
*/

static void size_test()
{
    std::map<int, void*> map1;
    LTK_LOG("map1: %d", sizeof(map1));
    std::function<void(void)> fn1;
    LTK_LOG("fn1: %d", sizeof(fn1));
    std::function<void(int, float, std::string)> fn2;
    LTK_LOG("fn2: %d", sizeof(fn2));
    LTK_LOG("Sprite: %d", sizeof(Sprite));
    LTK_LOG("RefCounted: %d",sizeof(RefCounted));
    std::forward_list<int> slist;
    LTK_LOG("forward_list: %d", sizeof(slist));
    std::list<int> dlist;
    LTK_LOG("std::list: %d", sizeof(dlist));
}

static void log_thread_test()
{
    auto tfunc = [](){
        for (int i = 0; i < 1000; i++) {
            LTK_LOG("log in thread test: [%d] %i", ::GetCurrentThreadId(), i);
        }
    };
    auto start_time = ltk::TickCount();
    std::thread tt1(tfunc);
    std::thread tt2(tfunc);
    std::thread tt3(tfunc);
    tt1.join();
    tt2.join();
    tt3.join();
    LTK_LOG("time: %d", ltk::TickCount() - start_time);
}

/*
static void array_list_test()
{
    CComPtr<IUnknown> cp;
    const size_t SIZE = 100;
    ArrayList<RefPtr<RefCounted>> list;
    for (size_t i = 0; i < SIZE; i++) {
        RefPtr<RefCounted> ptr; ptr.Attach(new RefCounted);
        list.PushBack(std::move(ptr));
    }
    ArrayList<RefPtr<RefCounted>> list2(std::move(list));
    LTK_ASSERT(list2.Length() == SIZE);
    LTK_ASSERT(list.Length() == 0);
    ArrayList<RefPtr<RefCounted>> list3(list2);
    ArrayList<RefPtr<RefCounted>> list4(list);
}

#define LTK_EXPORT_CURRENT_FUNCTION comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)

int luaopen_ltk(lua_State *L)
{
    #pragma LTK_EXPORT_CURRENT_FUNCTION
    
    //size_test();
    //vector_test();
    array_list_test();

    lua_atpanic(L, my_panic_func);

    LtkLogInit();
    // log_thread_test();
    LtkInitialize();
    ApiBindingInit(L);
    Window::RegisterWndClass();
    LuaRegisterClass<Window>(L, "LtkWindow");
    LuaRegisterClass<Sprite>(L, "LtkSprite");
    LuaRegisterClass<Button>(L, "LtkButton");
    LuaRegisterClass<BoxLayout>(L, "LtkBoxLayout");
    LuaRegisterClass<Label>(L, "LtkLabel");
    LuaRegisterClass<Serializer>(L, "LtkSerializer");
    LuaRegisterClass<StyleManager>(L, "LtkStyleManager");
    LuaRegisterClass<IconSprite>(L, "LtkIconSprite");
    LuaRegisterClass<TextureSprite>(L, "LtkTextureSprite");
    LuaRegisterClass<ImageSprite>(L, "LtkImageSprite");
    LuaRegisterClass<ScrollBar>(L, "LtkScrollBar");
    LuaRegisterClass<CheckBox>(L, "LtkCheckBox");
    LuaRegisterClass<RadioGroup>(L, "LtkRadioGroup");
    LuaRegisterClass<ListBox>(L, "LtkListBox");
    LuaRegisterClass<HeaderCtrl>(L, "LtkHeaderCtrl");

    LuaRegisterClass<LuaVector<unsigned char>>(L, LuaVectorScriptName<unsigned char>());
    LuaRegisterClass<LuaVector<unsigned short>>(L, LuaVectorScriptName<unsigned short>());
    LuaRegisterClass<LuaVector<unsigned long>>(L, LuaVectorScriptName<unsigned long>());

    LuaRegisterClass<LuaVector<signed char>>(L, LuaVectorScriptName<signed char>());
    LuaRegisterClass<LuaVector<signed short>>(L, LuaVectorScriptName<signed short>());
    LuaRegisterClass<LuaVector<signed long>>(L, LuaVectorScriptName<signed long>());

    LuaRegisterClass<LuaVector<float>>(L, LuaVectorScriptName<float>());
    LuaRegisterClass<LuaVector<double>>(L, LuaVectorScriptName<double>());

    ShadowFrame::Init();

    return 0;
}
*/
