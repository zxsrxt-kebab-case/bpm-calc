#include "elements.hpp"

#include "animation.hpp"

#include <map>
#include <algorithm>

ImColor construct_imcolor( int r, int g, int b, float alpha )
{
    return ImColor{ r / 255.f, g / 255.f, b / 255.f, alpha };
}

void render_text( ImVec2 pos, ImU32 color, const char* text, const char* text_end = ( char* ) 0, bool hide_text_after_hash = true )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    // Hide anything after a '##' string
    const char* text_display_end;
    if ( hide_text_after_hash )
    {
        text_display_end = ImGui::FindRenderedTextEnd( text, text_end );
    }
    else
    {
        if ( !text_end )
            text_end = text + strlen( text ); // FIXME-OPT
        text_display_end = text_end;
    }

    if ( text != text_display_end )
    {
        window->DrawList->AddText( g.Font, g.FontSize, pos, color, text, text_display_end );
        if ( g.LogEnabled )
            ImGui::LogRenderedText( &pos, text, text_display_end );
    }
}

bool checkbox_ex( const char* label, bool* v )
{
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );

    const float square_sz = GetFrameHeight( );

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb( pos, pos + ImVec2( square_sz + ( label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f ), label_size.y + style.FramePadding.y ) );
    ItemSize( total_bb, style.FramePadding.y );
    const bool is_visible = ItemAdd( total_bb, id );
    const bool is_multi_select = ( g.LastItemData.ItemFlags & ImGuiItemFlags_IsMultiSelect ) != 0;
    if ( !is_visible )
        if ( !is_multi_select || !g.BoxSelectState.UnclipMode || !g.BoxSelectState.UnclipRect.Overlaps( total_bb ) ) // Extra layer of "no logic clip" for box-select support
        {
            IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | ( *v ? ImGuiItemStatusFlags_Checked : 0 ) );
            return false;
        }

    bool checked = *v;
    if ( is_multi_select )
        MultiSelectItemHeader( id, &checked, NULL );

    bool hovered, held;
    bool pressed = ButtonBehavior( total_bb, id, &hovered, &held );

    if ( is_multi_select )
        MultiSelectItemFooter( id, &checked, &pressed );
    else if ( pressed )
        checked = !checked;

    if ( *v != checked )
    {
        *v = checked;
        pressed = true; // return value
        MarkItemEdited( id );
    }

    const ImRect check_bb( pos, pos + ImVec2( square_sz, square_sz ) );
    if ( is_visible )
    {
        if ( *v )
            window->DrawList->AddRectFilledMultiColor( pos + ImVec2( 2, 4 ), pos + ImVec2( 10, 12 ), construct_imcolor( 85, 135, 235, g.Style.Alpha ),
                                                       construct_imcolor( 85, 135, 235, g.Style.Alpha ), construct_imcolor( 30, 30, 30, g.Style.Alpha ), construct_imcolor( 30, 30, 30, g.Style.Alpha ) );
        else
        {
            if ( hovered )
                window->DrawList->AddRectFilledMultiColor( pos + ImVec2( 2, 4 ), pos + ImVec2( 10, 12 ),
                                                           construct_imcolor( 79, 79, 79, g.Style.Alpha ), construct_imcolor( 79, 79, 79, g.Style.Alpha ),
                                                           construct_imcolor( 46, 46, 46, g.Style.Alpha ), construct_imcolor( 46, 46, 46, g.Style.Alpha ) );
            else
                window->DrawList->AddRectFilledMultiColor( pos + ImVec2( 2, 4 ), pos + ImVec2( 10, 12 ),
                                                           construct_imcolor( 69, 69, 69, g.Style.Alpha ), construct_imcolor( 69, 69, 69, g.Style.Alpha ),
                                                           construct_imcolor( 36, 36, 36, g.Style.Alpha ), construct_imcolor( 36, 36, 36, g.Style.Alpha ) );
        }

        window->DrawList->AddRect( pos + ImVec2( 2, 4 ), pos + ImVec2( 10, 12 ), construct_imcolor( 1, 1, 1, g.Style.Alpha ) );
    }
    //window->DrawList->AddRect( total_bb.Min, total_bb.Max, ImColor( 255, 0, 0, 255 ) ); //total_bb
    //const auto height = total_bb.Max.y - total_bb.Min.y;
    //window->DrawList->AddLine( { total_bb.Min.x, total_bb.Min.y + height / 2 }, { total_bb.Min.x + 270, total_bb.Min.y + height / 2 }, IM_COL32( 255, 255, 255, 255 ) );
    if ( is_visible )
        render_text( pos + ImVec2( 22, 0 ), construct_imcolor( 180, 180, 180, g.Style.Alpha ), label );
    //window->DrawList->AddText( pos + ImVec2( 22, 0 ), construct_imcolor( 180, 180, 180, g.Style.Alpha ), label );

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | ( *v ? ImGuiItemStatusFlags_Checked : 0 ) );
    return pressed;
}

bool internal_scalar( const char* label, ImGuiDataType data_type, void* v, const void* v_min, const void* v_max, bool otstup, const char* format, float power, int remove_from_fmt )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const float w = ImClamp( window->Size.x - 64, 155.f, 200.f );

    const ImVec2 label_size = CalcTextSize( label, NULL, true );
    const ImRect frame_bb( window->DC.CursorPos + ImVec2( otstup ? 22 : 0, 0 ), window->DC.CursorPos + ImVec2( w + ( otstup ? 4 : 0 ), label_size.x > 0 ? label_size.y + 10 : 10 ) + ImVec2( ( otstup ? 10 : 0 ), 1 ) );
    const ImRect total_bb( frame_bb.Min, frame_bb.Max + ImVec2( label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f + 3 ) );

    ItemSize( total_bb, style.FramePadding.y );
    if ( !ItemAdd( total_bb, id, &frame_bb ) )
        return false;

    IM_ASSERT( data_type >= 0 && data_type < ImGuiDataType_COUNT );
    if ( format == NULL )
        format = DataTypeGetInfo( data_type )->PrintFmt;

    bool start_text_input = false;
    const bool hovered = ItemHoverable( total_bb, id, 0 );
    const bool clicked = hovered && IsMouseClicked( 0, ImGuiInputFlags_None, id );

    const bool make_active = ( clicked || g.NavActivateId == id );
    if ( make_active && clicked )
        SetKeyOwner( ImGuiKey_MouseLeft, id );

    if ( make_active )
    {
        SetActiveID( id, window );
        SetFocusID( id, window );
        FocusWindow( window );
        g.ActiveIdUsingNavDirMask |= ( 1 << ImGuiDir_Left ) | ( 1 << ImGuiDir_Right );
    }

    if ( std::string( label ).at( 0 ) != '#' && std::string( label ).at( 1 ) != '#' )
        render_text( frame_bb.Min, construct_imcolor( 180, 180, 180, g.Style.Alpha ), label );
    //window->DrawList->AddText( frame_bb.Min, construct_imcolor( 180, 180, 180, g.Style.Alpha ), label );

    ImRect slider_bb = ImRect( frame_bb.Min + ImVec2( 0, label_size.x > 0 ? label_size.y + 3 : 3 ), frame_bb.Max );

    if ( IsItemHovered( ) )
        window->DrawList->AddRectFilledMultiColor( slider_bb.Min, slider_bb.Max, construct_imcolor( 79, 79, 79, g.Style.Alpha ),
                                                   construct_imcolor( 79, 79, 79, g.Style.Alpha ), construct_imcolor( 46, 46, 46, g.Style.Alpha ), construct_imcolor( 46, 46, 46, g.Style.Alpha ) );
    else
        window->DrawList->AddRectFilledMultiColor( slider_bb.Min, slider_bb.Max, construct_imcolor( 69, 69, 69, g.Style.Alpha ),
                                                   construct_imcolor( 69, 69, 69, g.Style.Alpha ), construct_imcolor( 36, 36, 36, g.Style.Alpha ), construct_imcolor( 36, 36, 36, g.Style.Alpha ) );

    ImRect grab_bb;
    const bool value_changed = SliderBehavior( frame_bb, id, data_type, v, v_min, v_max, format, ImGuiSliderFlags_None, &grab_bb );
    if ( value_changed )
        MarkItemEdited( id );

    ImVec2 slider_end;

    if ( grab_bb.Max.x > grab_bb.Min.x )
    {

        slider_end = ImVec2( grab_bb.Max.x, slider_bb.Max.y );

        window->DrawList->AddRectFilledMultiColor( slider_bb.Min, slider_end,
                                                   construct_imcolor( 85, 135, 235, g.Style.Alpha ), construct_imcolor( 85, 135, 235, g.Style.Alpha ), construct_imcolor( 30, 30, 30, g.Style.Alpha ), construct_imcolor( 30, 30, 30, g.Style.Alpha ) );
        window->DrawList->AddRect( slider_bb.Min, slider_bb.Max, construct_imcolor( 3, 3, 3, g.Style.Alpha ) );
    }

    if ( data_type == ImGuiDataType_S32 )
        *( int* ) v -= remove_from_fmt;

    char value_buf[ 64 ];
    const char* value_buf_end = value_buf + DataTypeFormatString( value_buf, IM_ARRAYSIZE( value_buf ), data_type, v, format );
    auto textSize = CalcTextSize( value_buf );

    if ( data_type == ImGuiDataType_S32 )
        *( int* ) v += remove_from_fmt;

    window->DrawList->AddText( ImVec2{ slider_end.x, slider_end.y - textSize.y / 2 - 4 }, construct_imcolor( 180, 180, 180, g.Style.Alpha ), value_buf );

    //window->DrawList->AddRect( slider_bb.Min, slider_bb.Max, IM_COL32( 255, 0, 0, 255 ) );
    //window->DrawList->AddRect( frame_bb.Min, frame_bb.Max, IM_COL32( 0, 255, 0, 255 ) );
    //window->DrawList->AddRect( total_bb.Min, total_bb.Max, IM_COL32( 0, 0, 255, 255 ) );
    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, window->DC.ItemFlags );
    return value_changed;
}

void custom_separator_ex( ImGuiSeparatorFlags flags, ImU32 color, float thickness = 1.f )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return;

    ImGuiContext& g = *GImGui;
    IM_ASSERT( ImIsPowerOfTwo( flags & ( ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical ) ) );   // Check that only 1 option is selected
    IM_ASSERT( thickness > 0.0f );

    if ( flags & ImGuiSeparatorFlags_Vertical )
    {
        // Vertical separator, for menu bars (use current line height).
        float y1 = window->DC.CursorPos.y;
        float y2 = window->DC.CursorPos.y + window->DC.CurrLineSize.y;
        const ImRect bb( ImVec2( window->DC.CursorPos.x, y1 ), ImVec2( window->DC.CursorPos.x + thickness, y2 ) );
        ItemSize( ImVec2( thickness, 0.0f ) );
        if ( !ItemAdd( bb, 0 ) )
            return;

        // Draw
        window->DrawList->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_Separator ) );
        if ( g.LogEnabled )
            LogText( " |" );
    }
    else if ( flags & ImGuiSeparatorFlags_Horizontal )
    {
        // Horizontal Separator
        float x1 = window->DC.CursorPos.x;
        float x2 = window->WorkRect.Max.x;

        // Preserve legacy behavior inside Columns()
        // Before Tables API happened, we relied on Separator() to span all columns of a Columns() set.
        // We currently don't need to provide the same feature for tables because tables naturally have border features.
        ImGuiOldColumns* columns = ( flags & ImGuiSeparatorFlags_SpanAllColumns ) ? window->DC.CurrentColumns : NULL;
        if ( columns )
        {
            x1 = window->Pos.x + window->DC.Indent.x; // Used to be Pos.x before 2023/10/03
            x2 = window->Pos.x + window->Size.x;
            PushColumnsBackground( );
        }

        // We don't provide our width to the layout so that it doesn't get feed back into AutoFit
        // FIXME: This prevents ->CursorMaxPos based bounding box evaluation from working (e.g. TableEndCell)
        const float thickness_for_layout = ( thickness == 1.0f ) ? 0.0f : thickness; // FIXME: See 1.70/1.71 Separator() change: makes legacy 1-px separator not affect layout yet. Should change.
        const ImRect bb( ImVec2( x1, window->DC.CursorPos.y ), ImVec2( x2, window->DC.CursorPos.y + thickness ) );
        ItemSize( ImVec2( 0.0f, thickness_for_layout ) );

        if ( ItemAdd( bb, 0 ) )
        {
            // Draw
            window->DrawList->AddRectFilled( bb.Min, bb.Max, color );
            if ( g.LogEnabled )
                LogRenderedText( &bb.Min, "--------------------------------\n" );

        }
        if ( columns )
        {
            PopColumnsBackground( );
            columns->LineMinY = window->DC.CursorPos.y;
        }
    }
}
void custom_separator_multi_color_ex( ImGuiSeparatorFlags flags, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left, float thickness = 1.f )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return;

    ImGuiContext& g = *GImGui;
    IM_ASSERT( ImIsPowerOfTwo( flags & ( ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical ) ) );   // Check that only 1 option is selected
    IM_ASSERT( thickness > 0.0f );

    if ( flags & ImGuiSeparatorFlags_Vertical )
    {
        // Vertical separator, for menu bars (use current line height).
        float y1 = window->DC.CursorPos.y;
        float y2 = window->DC.CursorPos.y + window->DC.CurrLineSize.y;
        const ImRect bb( ImVec2( window->DC.CursorPos.x, y1 ), ImVec2( window->DC.CursorPos.x + thickness, y2 ) );
        ItemSize( ImVec2( thickness, 0.0f ) );
        if ( !ItemAdd( bb, 0 ) )
            return;

        // Draw
        window->DrawList->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_Separator ) );
        if ( g.LogEnabled )
            LogText( " |" );
    }
    else if ( flags & ImGuiSeparatorFlags_Horizontal )
    {
        // Horizontal Separator
        float x1 = window->DC.CursorPos.x;
        float x2 = window->WorkRect.Max.x;

        // Preserve legacy behavior inside Columns()
        // Before Tables API happened, we relied on Separator() to span all columns of a Columns() set.
        // We currently don't need to provide the same feature for tables because tables naturally have border features.
        ImGuiOldColumns* columns = ( flags & ImGuiSeparatorFlags_SpanAllColumns ) ? window->DC.CurrentColumns : NULL;
        if ( columns )
        {
            x1 = window->Pos.x + window->DC.Indent.x; // Used to be Pos.x before 2023/10/03
            x2 = window->Pos.x + window->Size.x;
            PushColumnsBackground( );
        }

        // We don't provide our width to the layout so that it doesn't get feed back into AutoFit
        // FIXME: This prevents ->CursorMaxPos based bounding box evaluation from working (e.g. TableEndCell)
        const float thickness_for_layout = ( thickness == 1.0f ) ? 0.0f : thickness; // FIXME: See 1.70/1.71 Separator() change: makes legacy 1-px separator not affect layout yet. Should change.
        const ImRect bb( ImVec2( x1, window->DC.CursorPos.y ), ImVec2( x2, window->DC.CursorPos.y + thickness ) );
        ItemSize( ImVec2( 0.0f, thickness_for_layout ) );

        if ( ItemAdd( bb, 0 ) )
        {
            // Draw
            //window->DrawList->AddRectFilled( bb.Min, bb.Max, color );
            window->DrawList->AddRectFilledMultiColor( bb.Min, bb.Max, col_upr_left, col_upr_right, col_bot_right, col_bot_left );
            if ( g.LogEnabled )
                LogRenderedText( &bb.Min, "--------------------------------\n" );

        }
        if ( columns )
        {
            PopColumnsBackground( );
            columns->LineMinY = window->DC.CursorPos.y;
        }
    }
}

bool color_button( const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, const ImVec2& size_arg )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID( desc_id );
    const float default_size = GetFrameHeight( );
    const ImVec2 size( size_arg.x == 0.0f ? default_size : size_arg.x, size_arg.y == 0.0f ? default_size : size_arg.y );
    const ImRect bb( window->DC.CursorPos + ImVec2{ 0, 3.5 }, window->DC.CursorPos + size + ImVec2{ 0, 3.5 } );
    ItemSize( bb, ( size.y >= default_size ) ? g.Style.FramePadding.y : 0.0f );
    if ( !ItemAdd( bb, id ) )
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    if ( flags & ImGuiColorEditFlags_NoAlpha )
        flags &= ~( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf );

    ImVec4 col_rgb = col;
    if ( flags & ImGuiColorEditFlags_InputHSV )
        ColorConvertHSVtoRGB( col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z );

    ImVec4 col_rgb_without_alpha( col_rgb.x, col_rgb.y, col_rgb.z, 1.0f );
    float grid_step = ImMin( size.x, size.y ) / 2.99f;
    float rounding = ImMin( g.Style.FrameRounding, grid_step * 0.5f );
    ImRect bb_inner = bb;
    float off = 0.0f;
    if ( ( flags & ImGuiColorEditFlags_NoBorder ) == 0 )
    {
        off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
        bb_inner.Expand( off );
    }
    if ( ( flags & ImGuiColorEditFlags_AlphaPreviewHalf ) && col_rgb.w < 1.0f )
    {
        float mid_x = IM_ROUND( ( bb_inner.Min.x + bb_inner.Max.x ) * 0.5f );
        RenderColorRectWithAlphaCheckerboard( window->DrawList, ImVec2( bb_inner.Min.x + grid_step, bb_inner.Min.y ), bb_inner.Max, GetColorU32( col_rgb ), grid_step, ImVec2( -grid_step + off, off ), rounding, ImDrawFlags_RoundCornersRight );
        window->DrawList->AddRectFilled( bb_inner.Min, ImVec2( mid_x, bb_inner.Max.y ), GetColorU32( col_rgb_without_alpha ), rounding, ImDrawFlags_RoundCornersLeft );
    }
    else
    {
        // Because GetColorU32() multiplies by the global style Alpha and we don't want to display a checkerboard if the source code had no alpha
        ImVec4 col_source = ( flags & ImGuiColorEditFlags_AlphaPreview ) ? col_rgb : col_rgb_without_alpha;
        if ( col_source.w < 1.0f )
            RenderColorRectWithAlphaCheckerboard( window->DrawList, bb_inner.Min, bb_inner.Max, GetColorU32( col_source ), grid_step, ImVec2( off, off ), rounding );
        else
            window->DrawList->AddRectFilled( bb_inner.Min, bb_inner.Max, GetColorU32( col_source ), rounding );
    }
    RenderNavCursor( bb, id );
    if ( ( flags & ImGuiColorEditFlags_NoBorder ) == 0 )
    {
        if ( g.Style.FrameBorderSize > 0.0f )
            RenderFrameBorder( bb.Min, bb.Max, rounding );
        else
            window->DrawList->AddRect( bb.Min, bb.Max, IM_COL32( 0, 0, 0, 255 ), rounding, 0, 1.f ); // Color button are often in need of some sort of border
    }

    // Drag and Drop Source
    // NB: The ActiveId test is merely an optional micro-optimization, BeginDragDropSource() does the same test.
    if ( g.ActiveId == id && !( flags & ImGuiColorEditFlags_NoDragDrop ) && BeginDragDropSource( ) )
    {
        if ( flags & ImGuiColorEditFlags_NoAlpha )
            SetDragDropPayload( IMGUI_PAYLOAD_TYPE_COLOR_3F, &col_rgb, sizeof( float ) * 3, ImGuiCond_Once );
        else
            SetDragDropPayload( IMGUI_PAYLOAD_TYPE_COLOR_4F, &col_rgb, sizeof( float ) * 4, ImGuiCond_Once );
        ColorButton( desc_id, col, flags );
        SameLine( );
        TextEx( "Color" );
        EndDragDropSource( );
    }

    // Tooltip
    if ( !( flags & ImGuiColorEditFlags_NoTooltip ) && hovered && IsItemHovered( ImGuiHoveredFlags_ForTooltip ) )
        ColorTooltip( desc_id, &col.x, flags & ( ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf ) );

    return pressed;
}

static void ColorEditRestoreHS( const float* col, float* H, float* S, float* V )
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT( g.ColorEditCurrentID != 0 );
    if ( g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32( ImVec4( col[ 0 ], col[ 1 ], col[ 2 ], 0 ) ) )
        return;

    // When S == 0, H is undefined.
    // When H == 1 it wraps around to 0.
    if ( *S == 0.0f || ( *H == 0.0f && g.ColorEditSavedHue == 1 ) )
        *H = g.ColorEditSavedHue;

    // When V == 0, S is undefined.
    if ( *V == 0.0f )
        *S = g.ColorEditSavedSat;
}

#define STB_TEXT_HAS_SELECTION(s)   ((s)->select_start != (s)->select_end)

bool color_edit_ex( const char* label, float col[ 4 ], ImGuiColorEditFlags flags )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float square_sz = GetFrameHeight( );
    const char* label_display_end = FindRenderedTextEnd( label );
    float w_full = CalcItemWidth( );
    g.NextItemData.ClearFlags( );

    BeginGroup( );
    PushID( label );
    const bool set_current_color_edit_id = ( g.ColorEditCurrentID == 0 );
    if ( set_current_color_edit_id )
        g.ColorEditCurrentID = window->IDStack.back( );

    // If we're not showing any slider there's no point in doing any HSV conversions
    const ImGuiColorEditFlags flags_untouched = flags;
    if ( flags & ImGuiColorEditFlags_NoInputs )
        flags = ( flags & ( ~ImGuiColorEditFlags_DisplayMask_ ) ) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

    // Context menu: display and modify options (before defaults are applied)
    if ( !( flags & ImGuiColorEditFlags_NoOptions ) )
        ColorEditOptionsPopup( col, flags );

    // Read stored options
    if ( !( flags & ImGuiColorEditFlags_DisplayMask_ ) )
        flags |= ( g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_ );
    if ( !( flags & ImGuiColorEditFlags_DataTypeMask_ ) )
        flags |= ( g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_ );
    if ( !( flags & ImGuiColorEditFlags_PickerMask_ ) )
        flags |= ( g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_ );
    if ( !( flags & ImGuiColorEditFlags_InputMask_ ) )
        flags |= ( g.ColorEditOptions & ImGuiColorEditFlags_InputMask_ );
    flags |= ( g.ColorEditOptions & ~( ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ ) );
    IM_ASSERT( ImIsPowerOfTwo( flags & ImGuiColorEditFlags_DisplayMask_ ) ); // Check that only 1 is selected
    IM_ASSERT( ImIsPowerOfTwo( flags & ImGuiColorEditFlags_InputMask_ ) );   // Check that only 1 is selected

    const bool alpha = ( flags & ImGuiColorEditFlags_NoAlpha ) == 0;
    const bool hdr = ( flags & ImGuiColorEditFlags_HDR ) != 0;
    const int components = alpha ? 4 : 3;
    const float w_button = ( flags & ImGuiColorEditFlags_NoSmallPreview ) ? 0.0f : ( square_sz + style.ItemInnerSpacing.x );
    const float w_inputs = ImMax( w_full - w_button, 1.0f );
    w_full = w_inputs + w_button;

    // Convert to the formats we need
    float f[ 4 ] = { col[ 0 ], col[ 1 ], col[ 2 ], alpha ? col[ 3 ] : 1.0f };
    if ( ( flags & ImGuiColorEditFlags_InputHSV ) && ( flags & ImGuiColorEditFlags_DisplayRGB ) )
        ColorConvertHSVtoRGB( f[ 0 ], f[ 1 ], f[ 2 ], f[ 0 ], f[ 1 ], f[ 2 ] );
    else if ( ( flags & ImGuiColorEditFlags_InputRGB ) && ( flags & ImGuiColorEditFlags_DisplayHSV ) )
    {
        // Hue is lost when converting from grayscale rgb (saturation=0). Restore it.
        ColorConvertRGBtoHSV( f[ 0 ], f[ 1 ], f[ 2 ], f[ 0 ], f[ 1 ], f[ 2 ] );
        ColorEditRestoreHS( col, &f[ 0 ], &f[ 1 ], &f[ 2 ] );
    }
    int i[ 4 ] = { IM_F32_TO_INT8_UNBOUND( f[ 0 ] ), IM_F32_TO_INT8_UNBOUND( f[ 1 ] ), IM_F32_TO_INT8_UNBOUND( f[ 2 ] ), IM_F32_TO_INT8_UNBOUND( f[ 3 ] ) };

    bool value_changed = false;
    bool value_changed_as_float = false;

    const ImVec2 pos = window->DC.CursorPos;
    const float inputs_offset_x = ( style.ColorButtonPosition == ImGuiDir_Left ) ? w_button : 0.0f;
    window->DC.CursorPos.x = pos.x + inputs_offset_x;



    ImGuiWindow* picker_active_window = NULL;
    if ( !( flags & ImGuiColorEditFlags_NoSmallPreview ) )
    {
        const float button_offset_x = ( ( flags & ImGuiColorEditFlags_NoInputs ) || ( style.ColorButtonPosition == ImGuiDir_Left ) ) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2( pos.x + button_offset_x, pos.y );

        const ImVec4 col_v4( col[ 0 ], col[ 1 ], col[ 2 ], alpha ? col[ 3 ] : 1.0f );
        if ( color_button( "##ColorButton", col_v4, flags, { 20, 10 } ) )
        {
            if ( !( flags & ImGuiColorEditFlags_NoPicker ) )
            {
                // Store current color and open a picker
                g.ColorPickerRef = col_v4;
                OpenPopup( "picker" );
                SetNextWindowPos( g.LastItemData.Rect.GetBL( ) + ImVec2( 0.0f, style.ItemSpacing.y ) );
            }
        }
        if ( !( flags & ImGuiColorEditFlags_NoOptions ) )
            OpenPopupOnItemClick( "context", ImGuiPopupFlags_MouseButtonRight );

        if ( BeginPopup( "picker" ) )
        {
            if ( g.CurrentWindow->BeginCount == 1 )
            {
                picker_active_window = g.CurrentWindow;
                if ( label != label_display_end )
                {
                    TextEx( label, label_display_end );
                    Spacing( );
                }
                ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                ImGuiColorEditFlags picker_flags = ( flags_untouched & picker_flags_to_forward ) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
                ImGui::SetNextItemWidth( square_sz * 12.0f ); // Use 256 + bar sizes?
                value_changed |= ColorPicker4( "##picker", col, picker_flags, &g.ColorPickerRef.x );
            }
            EndPopup( );
        }
    }

    if ( label != label_display_end && !( flags & ImGuiColorEditFlags_NoLabel ) )
    {
        // Position not necessarily next to last submitted button (e.g. if style.ColorButtonPosition == ImGuiDir_Left),
        // but we need to use SameLine() to setup baseline correctly. Might want to refactor SameLine() to simplify this.
        SameLine( 0.0f, style.ItemInnerSpacing.x );
        window->DC.CursorPos.x = pos.x + ( ( flags & ImGuiColorEditFlags_NoInputs ) ? w_button : w_full + style.ItemInnerSpacing.x );
        TextEx( label, label_display_end );
    }

    // Convert back
    if ( value_changed && picker_active_window == NULL )
    {
        if ( !value_changed_as_float )
            for ( int n = 0; n < 4; n++ )
                f[ n ] = i[ n ] / 255.0f;
        if ( ( flags & ImGuiColorEditFlags_DisplayHSV ) && ( flags & ImGuiColorEditFlags_InputRGB ) )
        {
            g.ColorEditSavedHue = f[ 0 ];
            g.ColorEditSavedSat = f[ 1 ];
            ColorConvertHSVtoRGB( f[ 0 ], f[ 1 ], f[ 2 ], f[ 0 ], f[ 1 ], f[ 2 ] );
            g.ColorEditSavedID = g.ColorEditCurrentID;
            g.ColorEditSavedColor = ColorConvertFloat4ToU32( ImVec4( f[ 0 ], f[ 1 ], f[ 2 ], 0 ) );
        }
        if ( ( flags & ImGuiColorEditFlags_DisplayRGB ) && ( flags & ImGuiColorEditFlags_InputHSV ) )
            ColorConvertRGBtoHSV( f[ 0 ], f[ 1 ], f[ 2 ], f[ 0 ], f[ 1 ], f[ 2 ] );

        col[ 0 ] = f[ 0 ];
        col[ 1 ] = f[ 1 ];
        col[ 2 ] = f[ 2 ];
        if ( alpha )
            col[ 3 ] = f[ 3 ];
    }

    if ( set_current_color_edit_id )
        g.ColorEditCurrentID = 0;
    PopID( );
    EndGroup( );

    // Drag and Drop Target
    // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
    if ( ( g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect ) && !( g.LastItemData.ItemFlags & ImGuiItemFlags_ReadOnly ) && !( flags & ImGuiColorEditFlags_NoDragDrop ) && BeginDragDropTarget( ) )
    {
        bool accepted_drag_drop = false;
        if ( const ImGuiPayload* payload = AcceptDragDropPayload( IMGUI_PAYLOAD_TYPE_COLOR_3F ) )
        {
            memcpy( ( float* ) col, payload->Data, sizeof( float ) * 3 ); // Preserve alpha if any //-V512 //-V1086
            value_changed = accepted_drag_drop = true;
        }
        if ( const ImGuiPayload* payload = AcceptDragDropPayload( IMGUI_PAYLOAD_TYPE_COLOR_4F ) )
        {
            memcpy( ( float* ) col, payload->Data, sizeof( float ) * components );
            value_changed = accepted_drag_drop = true;
        }

        // Drag-drop payloads are always RGB
        if ( accepted_drag_drop && ( flags & ImGuiColorEditFlags_InputHSV ) )
            ColorConvertRGBtoHSV( col[ 0 ], col[ 1 ], col[ 2 ], col[ 0 ], col[ 1 ], col[ 2 ] );
        EndDragDropTarget( );
    }

    // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
    if ( picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window )
        g.LastItemData.ID = g.ActiveId;

    if ( value_changed && g.LastItemData.ID != 0 ) // In case of ID collision, the second EndGroup() won't catch g.ActiveId
        MarkItemEdited( g.LastItemData.ID );
    return value_changed;
}

bool elements::checkbox( const char* label, bool* v )
{
    return checkbox_ex( label, v );
}

bool elements::slider_float( const char* label, float* v, float v_min, float v_max, bool otstup, const char* format, float power )
{
    return internal_scalar( label, ImGuiDataType_Float, v, &v_min, &v_max, otstup, format, power, 0 );
}
bool elements::slider_int( const char* label, int* v, int v_min, int v_max, bool otstup, const char* format, float power )
{
    return internal_scalar( label, ImGuiDataType_S32, v, &v_min, &v_max, otstup, format, power, 0 );
}

void elements::custom_separator( unsigned int color, float thickness )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if ( window->SkipItems )
        return;

    // Those flags should eventually be configurable by the user
    // FIXME: We cannot g.Style.SeparatorTextBorderSize for thickness as it relates to SeparatorText() which is a decorated separator, not defaulting to 1.0f.
    ImGuiSeparatorFlags flags = ( window->DC.LayoutType == ImGuiLayoutType_Horizontal ) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;

    // Only applies to legacy Columns() api as they relied on Separator() a lot.
    if ( window->DC.CurrentColumns )
        flags |= ImGuiSeparatorFlags_SpanAllColumns;

    custom_separator_ex( flags, color, thickness );
}

void elements::custom_separator_multi_color( unsigned int col_upr_left, unsigned int col_upr_right, unsigned int col_bot_right, unsigned int col_bot_left, float thickness )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if ( window->SkipItems )
        return;

    // Those flags should eventually be configurable by the user
    // FIXME: We cannot g.Style.SeparatorTextBorderSize for thickness as it relates to SeparatorText() which is a decorated separator, not defaulting to 1.0f.
    ImGuiSeparatorFlags flags = ( window->DC.LayoutType == ImGuiLayoutType_Horizontal ) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;

    // Only applies to legacy Columns() api as they relied on Separator() a lot.
    if ( window->DC.CurrentColumns )
        flags |= ImGuiSeparatorFlags_SpanAllColumns;

    custom_separator_multi_color_ex( flags, col_upr_left, col_upr_right, col_bot_right, col_bot_left, thickness );
}

void elements::custom_separator_two_color( unsigned int col_upr, unsigned int col_bot, float thickness )
{
    custom_separator_multi_color( col_upr, col_upr, col_bot, col_bot, thickness );
}

void custom_separator_text_ex( ImU32 separator_col, const char* label, const char* label_end, float extra_w )
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiStyle& style = g.Style;

    const ImVec2 label_size = CalcTextSize( label, label_end, false );
    const ImVec2 pos = window->DC.CursorPos;
    const ImVec2 padding = style.SeparatorTextPadding;

    const float separator_thickness = style.SeparatorTextBorderSize;
    const ImVec2 min_size( label_size.x + extra_w + padding.x * 2.0f, ImMax( label_size.y + padding.y * 2.0f, separator_thickness ) );
    const ImRect bb( pos, ImVec2( window->WorkRect.Max.x, pos.y + min_size.y ) );
    const float text_baseline_y = ImTrunc( ( bb.GetHeight( ) - label_size.y ) * style.SeparatorTextAlign.y + 0.99999f ); //ImMax(padding.y, ImFloor((style.SeparatorTextSize - label_size.y) * 0.5f));
    ItemSize( min_size, text_baseline_y );
    if ( !ItemAdd( bb, 0 ) )
        return;

    const float sep1_x1 = pos.x;
    const float sep2_x2 = bb.Max.x;
    const float seps_y = ImTrunc( ( bb.Min.y + bb.Max.y ) * 0.5f + 0.99999f );

    const float label_avail_w = ImMax( 0.0f, sep2_x2 - sep1_x1 - padding.x * 2.0f );
    const ImVec2 label_pos( pos.x + padding.x + ImMax( 0.0f, ( label_avail_w - label_size.x - extra_w ) * style.SeparatorTextAlign.x ), pos.y + text_baseline_y ); // FIXME-ALIGN

    // This allows using SameLine() to position something in the 'extra_w'
    window->DC.CursorPosPrevLine.x = label_pos.x + label_size.x;

    if ( label_size.x > 0.0f )
    {
        const float sep1_x2 = label_pos.x - style.ItemSpacing.x;
        const float sep2_x1 = label_pos.x + label_size.x + extra_w + style.ItemSpacing.x;
        if ( sep1_x2 > sep1_x1 && separator_thickness > 0.0f )
            window->DrawList->AddLine( ImVec2( sep1_x1, seps_y ), ImVec2( sep1_x2, seps_y ), separator_col, separator_thickness );
        if ( sep2_x2 > sep2_x1 && separator_thickness > 0.0f )
            window->DrawList->AddLine( ImVec2( sep2_x1, seps_y ), ImVec2( sep2_x2, seps_y ), separator_col, separator_thickness );
        if ( g.LogEnabled )
            LogSetNextTextDecoration( "---", NULL );
        RenderTextEllipsis( window->DrawList, label_pos, ImVec2( bb.Max.x, bb.Max.y + style.ItemSpacing.y ), bb.Max.x, bb.Max.x, label, label_end, &label_size );
    }
    else
    {
        if ( g.LogEnabled )
            LogText( "---" );
        if ( separator_thickness > 0.0f )
            window->DrawList->AddLine( ImVec2( sep1_x1, seps_y ), ImVec2( sep2_x2, seps_y ), separator_col, separator_thickness );
    }
}

void draw_two_color_line( ImDrawList* drawList, const ImVec2& p1, const ImVec2& p2, ImU32 col_first, ImU32 col_second, elements::sep_mode mode, float thickness )
{
    if ( col_first == col_second )
    {
        drawList->AddLine( p1, p2, col_first, thickness );
        return;
    }

    if ( ( ( col_first | col_second ) & IM_COL32_A_MASK ) == 0 )
        return;

    const ImVec2 uv = drawList->_Data->TexUvWhitePixel;
    drawList->PrimReserve( 6, 4 );
    drawList->PrimWriteIdx( ( ImDrawIdx ) ( drawList->_VtxCurrentIdx ) );
    drawList->PrimWriteIdx( ( ImDrawIdx ) ( drawList->_VtxCurrentIdx + 1 ) );
    drawList->PrimWriteIdx( ( ImDrawIdx ) ( drawList->_VtxCurrentIdx + 2 ) );
    drawList->PrimWriteIdx( ( ImDrawIdx ) ( drawList->_VtxCurrentIdx ) );
    drawList->PrimWriteIdx( ( ImDrawIdx ) ( drawList->_VtxCurrentIdx + 2 ) );
    drawList->PrimWriteIdx( ( ImDrawIdx ) ( drawList->_VtxCurrentIdx + 3 ) );
    drawList->PrimWriteVtx( ImVec2( p1.x, p1.y - thickness / 2 ), uv, col_first );
    drawList->PrimWriteVtx( ImVec2( p2.x, p1.y - thickness / 2 ), uv, mode == elements::sep_mode::sep_mode_vertical ? col_first : col_second );
    drawList->PrimWriteVtx( ImVec2( p2.x, p2.y + thickness / 2 ), uv, col_second );
    drawList->PrimWriteVtx( ImVec2( p1.x, p2.y + thickness / 2 ), uv, mode == elements::sep_mode::sep_mode_vertical ? col_second : col_first );
}

void custom_separator_text_two_col_ex( ImU32 col_upr, ImU32 col_bot, elements::sep_mode mode, const char* label, const char* label_end, float extra_w )
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiStyle& style = g.Style;

    const ImVec2 label_size = CalcTextSize( label, label_end, false );
    const ImVec2 pos = window->DC.CursorPos;
    const ImVec2 padding = style.SeparatorTextPadding;

    const float separator_thickness = style.SeparatorTextBorderSize;
    const ImVec2 min_size( label_size.x + extra_w + padding.x * 2.0f, ImMax( label_size.y + padding.y * 2.0f, separator_thickness ) );
    const ImRect bb( pos, ImVec2( window->WorkRect.Max.x, pos.y + min_size.y ) );
    const float text_baseline_y = ImTrunc( ( bb.GetHeight( ) - label_size.y ) * style.SeparatorTextAlign.y + 0.99999f ); //ImMax(padding.y, ImFloor((style.SeparatorTextSize - label_size.y) * 0.5f));
    ItemSize( min_size, text_baseline_y );
    if ( !ItemAdd( bb, 0 ) )
        return;

    const float sep1_x1 = pos.x;
    const float sep2_x2 = bb.Max.x;
    const float seps_y = ImTrunc( ( bb.Min.y + bb.Max.y ) * 0.5f + 0.99999f );

    const float label_avail_w = ImMax( 0.0f, sep2_x2 - sep1_x1 - padding.x * 2.0f );
    const ImVec2 label_pos( pos.x + padding.x + ImMax( 0.0f, ( label_avail_w - label_size.x - extra_w ) * style.SeparatorTextAlign.x ), pos.y + text_baseline_y ); // FIXME-ALIGN

    // This allows using SameLine() to position something in the 'extra_w'
    window->DC.CursorPosPrevLine.x = label_pos.x + label_size.x;

    if ( label_size.x > 0.0f )
    {
        const float sep1_x2 = label_pos.x - style.ItemSpacing.x;
        const float sep2_x1 = label_pos.x + label_size.x + extra_w + style.ItemSpacing.x;
        if ( sep1_x2 > sep1_x1 && separator_thickness > 0.0f )
        {
            draw_two_color_line( window->DrawList, ImVec2( sep1_x1, seps_y ), ImVec2( sep1_x2, seps_y ), mode == elements::sep_mode::sep_mode_vertical ? col_upr : col_bot, mode == elements::sep_mode::sep_mode_vertical ? col_bot : col_upr, mode, separator_thickness );
        }

        if ( sep2_x2 > sep2_x1 && separator_thickness > 0.0f )
        {
            draw_two_color_line( window->DrawList, ImVec2( sep2_x1, seps_y ), ImVec2( sep2_x2, seps_y ), col_upr, col_bot, mode, separator_thickness );
        }

        if ( g.LogEnabled )
            LogSetNextTextDecoration( "---", NULL );
        RenderTextEllipsis( window->DrawList, label_pos, ImVec2( bb.Max.x, bb.Max.y + style.ItemSpacing.y ), bb.Max.x, bb.Max.x, label, label_end, &label_size );
    }
    else
    {
        if ( g.LogEnabled )
            LogText( "---" );
        if ( separator_thickness > 0.0f )
            draw_two_color_line( window->DrawList, ImVec2( sep1_x1, seps_y ), ImVec2( sep2_x2, seps_y ), col_upr, col_bot, mode, separator_thickness );
    }
}

void elements::custom_separator_text( const char* label, unsigned int color )
{
    ImGuiWindow* window = ImGui::GetCurrentWindow( );
    if ( window->SkipItems )
        return;

    custom_separator_text_ex( color, label, ImGui::FindRenderedTextEnd( label ), 0.0f );
}

void elements::custom_separator_text_two_color( const char* label, unsigned int col_upr, unsigned int col_bot, sep_mode mode )
{
    ImGuiWindow* window = ImGui::GetCurrentWindow( );
    if ( window->SkipItems )
        return;

    custom_separator_text_two_col_ex( col_upr, col_bot, mode, label, ImGui::FindRenderedTextEnd( label ), 0.0f );
}

bool elements::color_picker( const char* name, float* color, bool alpha )
{
    ImGuiWindow* window = ImGui::GetCurrentWindow( );
    ImGuiStyle* style = &ImGui::GetStyle( );

    ImGui::SameLine( 0 + window->Size.x - 43 - 7 + 18.f );
    auto alphaSliderFlag = alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha;

    return color_edit_ex( std::string{ "##" }.append( name ).append( "Picker" ).c_str( ), color, alphaSliderFlag | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip );
}

static float calc_max_popup_height_from_item_count( int items_count )
{
    ImGuiContext& g = *GImGui;
    if ( items_count <= 0 )
        return FLT_MAX;
    return ( g.FontSize + g.Style.ItemSpacing.y ) * items_count - g.Style.ItemSpacing.y + ( g.Style.WindowPadding.y * 2 );
}

bool begin_combo_popup( ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags )
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    if ( !IsPopupOpen( popup_id, ImGuiPopupFlags_None ) )
    {
        g.NextWindowData.ClearFlags( );
        return false;
    }

    // Set popup size
    float w = bb.GetWidth( );
    if ( g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint )
    {
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax( g.NextWindowData.SizeConstraintRect.Min.x, w );
    }
    else
    {
        if ( ( flags & ImGuiComboFlags_HeightMask_ ) == 0 )
            flags |= ImGuiComboFlags_HeightRegular;
        IM_ASSERT( ImIsPowerOfTwo( flags & ImGuiComboFlags_HeightMask_ ) ); // Only one
        int popup_max_height_in_items = -1;
        if ( flags & ImGuiComboFlags_HeightRegular )     popup_max_height_in_items = 8;
        else if ( flags & ImGuiComboFlags_HeightSmall )  popup_max_height_in_items = 4;
        else if ( flags & ImGuiComboFlags_HeightLarge )  popup_max_height_in_items = 20;
        ImVec2 constraint_min( 0.0f, 0.0f ), constraint_max( FLT_MAX, FLT_MAX );
        if ( ( g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize ) == 0 || g.NextWindowData.SizeVal.x <= 0.0f ) // Don't apply constraints if user specified a size
            constraint_min.x = w;
        if ( ( g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize ) == 0 || g.NextWindowData.SizeVal.y <= 0.0f )
            constraint_max.y = calc_max_popup_height_from_item_count( popup_max_height_in_items );
        SetNextWindowSizeConstraints( constraint_min, constraint_max - ImVec2( 0, 5 ) );
    }

    // This is essentially a specialized version of BeginPopupEx()
    char name[ 16 ];
    ImFormatString( name, IM_ARRAYSIZE( name ), "##Combo_%02d", g.BeginComboDepth ); // Recycle windows based on depth

    // Set position given a custom constraint (peak into expected window size so we can position it)
    // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
    // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
    if ( ImGuiWindow* popup_window = FindWindowByName( name ) )
        if ( popup_window->WasActive )
        {
            // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
            ImVec2 size_expected = CalcWindowNextAutoFitSize( popup_window );
            popup_window->AutoPosLastDirection = ( flags & ImGuiComboFlags_PopupAlignLeft ) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
            ImRect r_outer = GetPopupAllowedExtentRect( popup_window );
            ImVec2 pos = FindBestWindowPosForPopupEx( bb.GetBL( ), size_expected, &popup_window->AutoPosLastDirection, r_outer, bb, ImGuiPopupPositionPolicy_ComboBox );
            SetNextWindowPos( pos );
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
    PushStyleVarX( ImGuiStyleVar_WindowPadding, g.Style.FramePadding.x ); // Horizontally align ourselves with the framed text
    bool ret = Begin( name, NULL, window_flags );
    PopStyleVar( );
    if ( !ret )
    {
        EndPopup( );
        IM_ASSERT( 0 );   // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    g.BeginComboDepth++;
    return true;
}

void render_custom_arrow( ImDrawList* draw, ImVec2 p_min, ImU32 col, ImGuiDir dir, float scale )
{
    ImGuiContext& g = *GImGui;

    const float h = g.FontSize * 1.00f;
    float r = h * 0.40f * scale;
    ImVec2 center = p_min + ImVec2( h * 0.50f, h * 0.50f * scale );

    ImVec2 a, b, c;
    switch ( dir )
    {
        case ImGuiDir_Up:
        case ImGuiDir_Down:
            if ( dir == ImGuiDir_Up ) r = -r;
            a = ImVec2( +0.000f, +0.750f ) * r;
            b = ImVec2( -0.866f, -0.750f ) * r;
            c = ImVec2( +0.866f, -0.750f ) * r;
            break;
        case ImGuiDir_Left:
        case ImGuiDir_Right:
            if ( dir == ImGuiDir_Left ) r = -r;
            a = ImVec2( +0.750f, +0.000f ) * r;
            b = ImVec2( -0.750f, +0.866f ) * r;
            c = ImVec2( -0.750f, -0.866f ) * r;
            break;
        case ImGuiDir_None:
        case ImGuiDir_COUNT:
            IM_ASSERT( 0 );
            break;
    }

    draw->AddTriangleFilled( center + a, center + b, center + c, col );
}

bool custom_begin_combo( const char* label, const char* preview_value, bool otstup = false, ImGuiComboFlags flags = 0 )
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags( ); // We behave like Begin() and need to consume those values
    if ( window->SkipItems )
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    IM_ASSERT( ( flags & ( ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview ) ) != ( ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview ) ); // Can't use both flags together
    if ( flags & ImGuiComboFlags_WidthFitPreview )
        IM_ASSERT( ( flags & ( ImGuiComboFlags_NoPreview | ( ImGuiComboFlags ) ImGuiComboFlags_CustomPreview ) ) == 0 );

    const float arrow_size = ( flags & ImGuiComboFlags_NoArrowButton ) ? 0.0f : GetFrameHeight( );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );
    const float preview_width = ( ( flags & ImGuiComboFlags_WidthFitPreview ) && ( preview_value != NULL ) ) ? CalcTextSize( preview_value, NULL, true ).x : 0.0f;
    const float w = ( flags & ImGuiComboFlags_NoPreview ) ? arrow_size : ( ( flags & ImGuiComboFlags_WidthFitPreview ) ? ( arrow_size + preview_width + style.FramePadding.x * 2.0f ) : CalcItemWidth( ) );
    ImRect bb( window->DC.CursorPos + ImVec2( otstup ? 22 : 0, 0 ), window->DC.CursorPos + ImVec2( w + ( otstup ? 4 : 0 ), label_size.x > 0 ? label_size.y + 10 : 10 ) + ImVec2( ( otstup ? 10 : 0 ), 10 ) );
    ImRect total_bb( bb.Min, bb.Max );
    ItemSize( total_bb, style.FramePadding.y );
    if ( !ItemAdd( total_bb, id, &bb ) )
        return false;


    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );
    const ImGuiID popup_id = ImHashStr( "##ComboPopup", 0, id );
    bool popup_open = IsPopupOpen( popup_id, ImGuiPopupFlags_None );
    if ( pressed && !popup_open )
    {
        OpenPopupEx( popup_id, ImGuiPopupFlags_None );
        popup_open = true;
    }

    //ImRect slider_bb = ImRect( frame_bb.Min + ImVec2( 0, label_size.x > 0 ? label_size.y + 3 : 3 ), frame_bb.Max );

    ImRect combo_bb = ImRect( bb.Min + ImVec2( 0, label_size.y > 0 ? label_size.y + 3 : 3 ), bb.Max );

    // Render shape
    const ImU32 frame_col = hovered ? ImColor( 50, 50, 50, 255 ) : ImColor( 40, 40, 40, 255 );
    const float value_x2 = ImMax( combo_bb.Min.x, combo_bb.Max.x - arrow_size );
    RenderNavCursor( combo_bb, id );
    if ( !( flags & ImGuiComboFlags_NoPreview ) )
        window->DrawList->AddRectFilled( combo_bb.Min, combo_bb.Max, frame_col );


    if ( !( flags & ImGuiComboFlags_NoArrowButton ) )
    {
        if ( value_x2 + arrow_size - style.FramePadding.x <= bb.Max.x )
            render_custom_arrow( window->DrawList, ImVec2( value_x2 + style.FramePadding.y, combo_bb.Min.y + style.FramePadding.y + 2.f ), ImColor( 100, 100, 100, 255 ), popup_open ? ImGuiDir_Up : ImGuiDir_Down, 0.7f );
    }
    window->DrawList->AddRect( combo_bb.Min, combo_bb.Max, ImColor( 0, 0, 0, 255 ) );
    window->DrawList->AddRect( combo_bb.Min + ImVec2( 1, 1 ), combo_bb.Max - ImVec2( 1, 1 ), ImColor( 50, 50, 50, 255 ) );
    //RenderFrameBorder( bb.Min, bb.Max, style.FrameRounding );

    // Custom preview
    if ( flags & ImGuiComboFlags_CustomPreview )
    {
        g.ComboPreviewData.PreviewRect = ImRect( combo_bb.Min.x, combo_bb.Min.y, value_x2, combo_bb.Max.y );
        IM_ASSERT( preview_value == NULL || preview_value[ 0 ] == 0 );
        preview_value = NULL;
    }

    // Render preview and label
    if ( preview_value != NULL && !( flags & ImGuiComboFlags_NoPreview ) )
    {
        if ( g.LogEnabled )
            LogSetNextTextDecoration( "{", "}" );
        RenderTextClipped( combo_bb.Min + style.FramePadding, ImVec2( value_x2, combo_bb.Max.y ), preview_value, NULL, NULL );
    }
    if ( label_size.x > 0 )
        render_text( ImVec2( combo_bb.Min.x, combo_bb.Min.y - label_size.y ), construct_imcolor( 180, 180, 180, g.Style.Alpha ), label );

    //window->DrawList->AddRect( bb.Min, bb.Max, ImColor( 0, 255, 0, 255 ) );
    //window->DrawList->AddRect( combo_bb.Min, combo_bb.Max, ImColor( 0, 0, 255, 255 ) );
    //window->DrawList->AddRect( total_bb.Min, total_bb.Max, ImColor( 255, 0, 0, 255 ) );

    if ( !popup_open )
        return false;

    g.NextWindowData.Flags = backup_next_window_data_flags;
    return begin_combo_popup( popup_id, ImRect( bb.Min, bb.Max + ImVec2( 0, 3 ) ), flags );
}

void render_text_clipped_ex( ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, ImU32 col, const char* text, const char* text_display_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2( 0, 0 ), const ImRect* clip_rect = NULL )
{
    using namespace ImGui;
    // Perform CPU side clipping for single clipped element to avoid using scissor state
    ImVec2 pos = pos_min;
    const ImVec2 text_size = text_size_if_known ? *text_size_if_known : CalcTextSize( text, text_display_end, false, 0.0f );

    const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
    const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;
    bool need_clipping = ( pos.x + text_size.x >= clip_max->x ) || ( pos.y + text_size.y >= clip_max->y );
    if ( clip_rect ) // If we had no explicit clipping rectangle then pos==clip_min
        need_clipping |= ( pos.x < clip_min->x ) || ( pos.y < clip_min->y );

    // Align whole block. We should defer that to the better rendering function when we'll have support for individual line alignment.
    if ( align.x > 0.0f ) pos.x = ImMax( pos.x, pos.x + ( pos_max.x - pos.x - text_size.x ) * align.x );
    if ( align.y > 0.0f ) pos.y = ImMax( pos.y, pos.y + ( pos_max.y - pos.y - text_size.y ) * align.y );

    // Render
    if ( need_clipping )
    {
        ImVec4 fine_clip_rect( clip_min->x, clip_min->y, clip_max->x, clip_max->y );
        draw_list->AddText( NULL, 0.0f, pos, col, text, text_display_end, 0.0f, &fine_clip_rect );
    }
    else
    {
        draw_list->AddText( NULL, 0.0f, pos, col, text, text_display_end, 0.0f, NULL );
    }
}

void render_text_clipped( const ImVec2& pos_min, const ImVec2& pos_max, ImU32 col, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2( 0, 0 ), const ImRect* clip_rect = NULL )
{
    // Hide anything after a '##' string
    const char* text_display_end = ImGui::FindRenderedTextEnd( text, text_end );
    const int text_len = ( int ) ( text_display_end - text );
    if ( text_len == 0 )
        return;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    render_text_clipped_ex( window->DrawList, pos_min, pos_max, col, text, text_display_end, text_size_if_known, align, clip_rect );
    if ( g.LogEnabled )
        ImGui::LogRenderedText( &pos_min, text, text_display_end );
}

void render_frame( ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool borders, float rounding )
{

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled( p_min, p_max, fill_col, rounding );
    const float border_size = g.Style.FrameBorderSize;
    if ( borders && border_size > 0.0f )
    {
        window->DrawList->AddRect( p_min + ImVec2( 1, 1 ), p_max + ImVec2( 1, 1 ), ImColor( 0, 0, 0, 200 ), rounding, 0, border_size );
        window->DrawList->AddRect( p_min, p_max, ImColor( 0, 0, 0, 255 ), rounding, 0, border_size );
    }
}

bool selectable_ex( const char* label, bool selected, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2( 0, 0 ) )
{
    using namespace ImGui;

    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    ImGuiID id = window->GetID( label );
    ImVec2 label_size = CalcTextSize( label, NULL, true );
    ImVec2 size( size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y );
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize( size, 0.0f );

    // Fill horizontal space
    // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
    const bool span_all_columns = ( flags & ImGuiSelectableFlags_SpanAllColumns ) != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
    if ( size_arg.x == 0.0f || ( flags & ImGuiSelectableFlags_SpanAvailWidth ) )
        size.x = ImMax( label_size.x, max_x - min_x );

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos - ImVec2( 0, 1 );
    const ImVec2 text_max( min_x + size.x, pos.y + size.y );

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    // FIXME: Not part of layout so not included in clipper calculation, but ItemSize currently doesn't allow offsetting CursorPos.
    ImRect bb( min_x - 5, pos.y, text_max.x + 5, text_max.y );
    if ( ( flags & ImGuiSelectableFlags_NoPadWithHalfSpacing ) == 0 )
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_TRUNC( spacing_x * 0.50f );
        const float spacing_U = IM_TRUNC( spacing_y * 0.50f );
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += ( spacing_x - spacing_L );
        bb.Max.y += ( spacing_y - spacing_U );
    }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

    const bool disabled_item = ( flags & ImGuiSelectableFlags_Disabled ) != 0;
    const ImGuiItemFlags extra_item_flags = disabled_item ? ( ImGuiItemFlags ) ImGuiItemFlags_Disabled : ImGuiItemFlags_None;
    bool is_visible;
    if ( span_all_columns )
    {
        // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackgroundChannel for every Selectable..
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        is_visible = ItemAdd( bb, id, NULL, extra_item_flags );
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }
    else
    {
        is_visible = ItemAdd( bb, id, NULL, extra_item_flags );
    }

    const bool is_multi_select = ( g.LastItemData.ItemFlags & ImGuiItemFlags_IsMultiSelect ) != 0;
    if ( !is_visible )
        if ( !is_multi_select || !g.BoxSelectState.UnclipMode || !g.BoxSelectState.UnclipRect.Overlaps( bb ) ) // Extra layer of "no logic clip" for box-select support (would be more overhead to add to ItemAdd)
            return false;

    const bool disabled_global = ( g.CurrentItemFlags & ImGuiItemFlags_Disabled ) != 0;
    if ( disabled_item && !disabled_global ) // Only testing this as an optimization
        BeginDisabled( );
    // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
    // which would be advantageous since most selectable are not selected.
    if ( span_all_columns )
    {
        if ( g.CurrentTable )
            TablePushBackgroundChannel( );
        else if ( window->DC.CurrentColumns )
            PushColumnsBackground( );
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
        g.LastItemData.ClipRect = window->ClipRect;
    }

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    ImGuiButtonFlags button_flags = 0;
    if ( flags & ImGuiSelectableFlags_NoHoldingActiveID )
    {
        button_flags |= ImGuiButtonFlags_NoHoldingActiveId;
    }
    if ( flags & ImGuiSelectableFlags_NoSetKeyOwner )
    {
        button_flags |= ImGuiButtonFlags_NoSetKeyOwner;
    }
    if ( flags & ImGuiSelectableFlags_SelectOnClick )
    {
        button_flags |= ImGuiButtonFlags_PressedOnClick;
    }
    if ( flags & ImGuiSelectableFlags_SelectOnRelease )
    {
        button_flags |= ImGuiButtonFlags_PressedOnRelease;
    }
    if ( flags & ImGuiSelectableFlags_AllowDoubleClick )
    {
        button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    }
    if ( ( flags & ImGuiSelectableFlags_AllowOverlap ) || ( g.LastItemData.ItemFlags & ImGuiItemFlags_AllowOverlap ) )
    {
        button_flags |= ImGuiButtonFlags_AllowOverlap;
    }

    // Multi-selection support (header)
    const bool was_selected = selected;
    if ( is_multi_select )
    {
        // Handle multi-select + alter button flags for it
        MultiSelectItemHeader( id, &selected, &button_flags );
    }

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, button_flags );

    // Multi-selection support (footer)
    if ( is_multi_select )
    {
        MultiSelectItemFooter( id, &selected, &pressed );
    }
    else
    {
        if ( ( flags & ImGuiSelectableFlags_SelectOnNav ) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId )
            if ( g.NavJustMovedToId == id )
                selected = pressed = true;
    }

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with keyboard/gamepad
    if ( pressed || ( hovered && ( flags & ImGuiSelectableFlags_SetNavIdOnHover ) ) )
    {
        if ( !g.NavHighlightItemUnderNav && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent )
        {
            SetNavID( id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel( window, bb ) ); // (bb == NavRect)
            if ( g.IO.ConfigNavCursorVisibleAuto )
                g.NavCursorVisible = false;
        }
    }
    if ( pressed )
        MarkItemEdited( id );

    if ( selected != was_selected )
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    // Render
    if ( is_visible )
    {
        const bool highlighted = hovered || ( flags & ImGuiSelectableFlags_Highlight );
        if ( highlighted )
        {
            // Between 1.91.0 and 1.91.4 we made selected Selectable use an arbitrary lerp between _Header and _HeaderHovered. Removed that now. (#8106)
            ImU32 col = ImColor( 25, 25, 25, 255 );
            render_frame( bb.Min, bb.Max, col, false, 0.0f );
        }
        if ( g.NavId == id )
        {
            ImGuiNavRenderCursorFlags nav_render_cursor_flags = ImGuiNavRenderCursorFlags_Compact | ImGuiNavRenderCursorFlags_NoRounding;
            if ( is_multi_select )
                nav_render_cursor_flags |= ImGuiNavRenderCursorFlags_AlwaysDraw; // Always show the nav rectangle
            RenderNavCursor( bb, id, nav_render_cursor_flags );
        }
    }

    if ( span_all_columns )
    {
        if ( g.CurrentTable )
            TablePopBackgroundChannel( );
        else if ( window->DC.CurrentColumns )
            PopColumnsBackground( );
    }
    if ( is_visible )
        render_text_clipped( text_min, text_max, selected ? ImColor( 85, 135, 235, 255 ) : ImColor( 255, 255, 255, 255 ), label, NULL, &label_size, style.SelectableTextAlign, &bb );

    // Automatically close popups
    if ( pressed && ( window->Flags & ImGuiWindowFlags_Popup ) && !( flags & ImGuiSelectableFlags_NoAutoClosePopups ) && ( g.LastItemData.ItemFlags & ImGuiItemFlags_AutoClosePopups ) )
        CloseCurrentPopup( );

    if ( disabled_item && !disabled_global )
        EndDisabled( );



    // Selectable() always returns a pressed state!
    // Users of BeginMultiSelect()/EndMultiSelect() scope: you may call ImGui::IsItemToggledSelection() to retrieve
    // selection toggle, only useful if you need that state updated (e.g. for rendering purpose) before reaching EndMultiSelect().
    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
    return pressed; //-V1020
}

bool selectable_ex( const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2( 0, 0 ) )
{
    if ( selectable_ex( label, *p_selected, flags, size_arg ) )
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

bool elements::combo( const char* label, int* current_item, std::vector<std::string> items, bool otstup )
{
    *current_item = ImClamp( *current_item, 0, int( items.size( ) - 1 ) );

    int old_item = *current_item;
    ImGui::PushStyleColor( ImGuiCol_Border, ImColor( 0, 0, 0, 255 ).Value );
    ImGui::PushStyleColor( ImGuiCol_PopupBg, ImColor( 40, 40, 40, 255 ).Value );
    if ( custom_begin_combo( *current_item == -1 ? "none" : label, items.at( *current_item ).c_str( ), otstup ) )
    {
        for ( int i = 0; i < items.size( ); i++ )
            if ( selectable_ex( items.at( i ).c_str( ), *current_item == i ) )
                *current_item = i;

        ImGui::EndCombo( );
    }
    ImGui::PopStyleColor( 2 );
    return old_item != *current_item;
}

bool elements::mcombo( const char* label, std::unordered_map<std::string, bool*> data, bool otstup )
{
    std::unordered_map<std::string, bool> old_state;

    for ( const auto& [name, ptr] : data )
    {
        old_state[ name ] = *ptr;
    }

    auto count_selected = [ &data ]( ) -> int {
        int count = 0;
        for ( const auto& [_, ptr] : data )
        {
            if ( *ptr ) count++;
        }
        return count;
        };

    std::string preview_text;
    int selected_count = count_selected( );
    if ( selected_count > 0 )
    {
        preview_text = std::to_string( selected_count ) + " ...";
    }
    else
    {
        preview_text = "none";
    }

    if ( custom_begin_combo( label, preview_text.c_str( ), otstup ) )
    {
        for ( auto& [name, ptr] : data )
        {
            selectable_ex( name.c_str( ), ptr, ImGuiSelectableFlags_DontClosePopups );
        }
        ImGui::EndCombo( );
    }

    for ( const auto& [name, ptr] : data )
    {
        if ( old_state[ name ] != *ptr )
        {
            return true;
        }
    }
    return false;
}

bool button_ex( const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );

    ImVec2 pos = window->DC.CursorPos;
    if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ItemSize( size, style.FramePadding.y );
    if ( !ItemAdd( bb, id ) )
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, flags );

    // Render
    //const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavCursor( bb, id );
    RenderFrame( bb.Min, bb.Max, held && hovered ? ImColor( 70, 70, 70, 255 ) : ImColor( 40, 40, 40, 255 ), 0, style.FrameRounding );

    window->DrawList->AddRect( bb.Min, bb.Max, ImColor( 0, 0, 0, 255 ) );
    window->DrawList->AddRect( bb.Min + ImVec2( 1, 1 ), bb.Max - ImVec2( 1, 1 ), ImColor( 50, 50, 50, 255 ) );

    if ( g.LogEnabled )
        LogSetNextTextDecoration( "[", "]" );

    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb );

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
    return pressed;
}

bool elements::button( const char* label, const ImVec2& size_arg )
{
    return button_ex( label, size_arg, ImGuiButtonFlags_None );
}

bool tab_button_ex( const char* label, bool selected, const ImVec2& size_arg, ImGuiButtonFlags flags )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );

    ImVec2 pos = window->DC.CursorPos;
    if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ItemSize( size, style.FramePadding.y );
    if ( !ItemAdd( bb, id ) )
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, flags );

    // Render
    //const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavCursor( bb, id );
    //RenderFrame(bb.Min, bb.Max, held&&hovered ? ImColor(45,45,45): ImColor(40,40,40), 0, style.FrameRounding);

    ImU32 col_upr = held && hovered ? ImColor( 70, 70, 70, 255 ) : ImColor( 55, 55, 55, 255 );

    ImU32 col_bot = held && hovered ? ImColor( 45, 45, 45, 255 ) : ImColor( 30, 30, 30, 250 );

    window->DrawList->AddRectFilledMultiColor( bb.Min, bb.Max, col_upr, col_upr, col_bot, col_bot );

    window->DrawList->AddRect( bb.Min, bb.Max, ImColor( 0, 0, 0, 255 ) );
    window->DrawList->AddRect( bb.Min + ImVec2( 1, 1 ), bb.Max - ImVec2( 1, 1 ), ImColor( 50, 50, 50, 255 ) );

    if ( g.LogEnabled )
        LogSetNextTextDecoration( "[", "]" );

    render_text_clipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, selected ? ImColor( 85, 135, 235, 255 ) : ImColor( 170, 170, 170, 255 ), label, NULL, &label_size, style.ButtonTextAlign, &bb );

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
    return pressed;
}

bool elements::tab_button( const char* label, bool selected, const ImVec2& size_arg )
{
    return tab_button_ex( label, selected, size_arg, ImGuiButtonFlags_None );
}
