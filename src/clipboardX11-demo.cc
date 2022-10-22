#include "clipboardX11.cc"
#include <iostream>
#include <string>
#include <vector>

#define PASTE_CANDIDATE "paste me"

void printVector(std::vector<Atom>& vec) {
    
    std::cout << "[CLIP_LIB] Target atoms: [ ";
    for (std::size_t a = 0; a < vec.size(); a++) {
        std::cout << XGetAtomName(ClipLibX11::PrimaryDisplay, vec[a]) << " ";
    }
    std::cout << ']' << std::endl;
}

int main() {
    std::cout << "[CLIP_LIB] Testing started" << std::endl;
    std::cout << "[CLIP_LIB] Clipboard owner: " << XGetSelectionOwner(ClipLibX11::PrimaryDisplay, ClipLibX11::ClipAtom) << std::endl;
    std::vector<Atom> targets;
    std::string paste(PASTE_CANDIDATE);

    ClipLibX11::GetClipboardTargets(targets);
    printVector(targets);

    std::cout << "[CLIP_LIB] Current clipboard text: " << ClipLibX11::GetSelectionAsUtf8() << std::endl;

    std::cout << "[CLIP_LIB] Setting Text to: " << PASTE_CANDIDATE << std::endl;
    bool pasteStatus = ClipLibX11::SetClipboardText(paste.data());
    std::cout << "[CLIP_LIB] SetClipboardText status: " << ((pasteStatus) ? "good" : "bad") << std::endl;
    ClipLibX11::GetClipboardTargets(targets);
    printVector(targets);
    std::cout << "[CLIP_LIB] Check clipboard text: " << ClipLibX11::GetSelectionAsUtf8() << std::endl;
    ClipLibX11::RelinquishClipboard();
}