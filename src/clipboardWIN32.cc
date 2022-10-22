#pragma once

#define UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <locale>
#include <codecvt>
#include <cstring>

namespace ClipLibWin {
    /**
     * @brief Get clipboard data as a string.
     * @return std::string Returns a string of the copied text. If the function fails, resulting string will be empty.
     */
    std::string GetClipboardText() {

        if (!IsClipboardFormatAvailable(CF_TEXT) || !OpenClipboard(NULL)) return std::string();
        HANDLE hData = GetClipboardData(CF_TEXT);
        // Remember that this will likely be UTF16
        char* hMem = (char*) GlobalLock(hData);
        std::wstring wStr(hMem);
        GlobalUnlock(hData);
        CloseClipboard()
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(wStr);
    }
    /**
     * @brief Set text on the clipboard.
     * @param text String containing the data to set. Note that this function will copy data returned by std::string::c_str into the clipboard.
     * @return Returns true if successful, false otherwise.
     */
    bool SetClipboardText(std::string& const text) {
        if (!OpenClipboard(NULL) && !EmptyClipboard()) return false;
        char* ptrText = text.c_str();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, strlen(ptrText) + 1);
        auto ptrMem = GlobalLock(hMem);
        CopyMemory(ptrMem, ptrText, strlen(ptrText) + 1);
        SetClipboardData(CF_TEXT, hMem);
        GlobalUnlock(hMem);
        CloseClipboard();
        return true;
    }
    /**
     * @brief Internal function to determine if a specified format can be provided for current clipboard data.
     * @param format A standard clipboard format. Can use bitwise OR to combine several formats.
     * @param matchAll Set to true if all formats must be available.
     * @return Returns true if the current clipboard can support the specified format.
     */
    bool Internal_ClipboardHasFormat(uint format, bool matchAll) {
        auto formatType = EnumClipboardFormats(0u);
        auto available = 0u;
        while (formatType != format || formatType != 0) {
            available = available | formatType;
            formatType = EnumClipboardFormats(formatType);
        }
        if (matchAll) {
            return (format & available) >= format;
        } else {
            return (format & available) != 0u;
        }
    }
    /**
     * @brief Check if clipboard format CF_TEXT is available.
     * @return True if text format is available, false otherwise.
     */
    bool ClipboardHasText() {
        return Internal_ClipboardHasFormat(CF_TEXT, false);
    }

    bool ClipboardHasUnicodeText() {
        return Internal_ClipboardHasFormat(CF_UNICODETEXT, false);
    }
}