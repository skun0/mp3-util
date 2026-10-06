// MP3 Utils
// Developed by Skuno (yaxiistarks)
// If you need any help or find any bug, consider reaching me on tg @yaxiistarks or opening an issue on github :)
// https://github.com/skun0/mp3-util

// In the next update: Automatic file rename, Images, WinUi3???
#include <iostream>
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#pragma comment(lib, "Comdlg32.lib")
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <chrono>
#include <ctime>
void clear() {
    system("cls");

}

void thankyou() {
    clear();
    std::cout << "Thank you for using MP3 Util by skun0.";
}

int getCurrentYear() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};
    localtime_s(&localTime, &time);
    return 1900 + localTime.tm_year;
}

std::wstring SelectMP3File() {
    wchar_t fileName[MAX_PATH] = {};
    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter =
        L"MP3 Files (*.mp3)\0*.mp3\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameW(&ofn))
        return fileName;
    return L""; 
}

void read(const std::wstring& path) {
    clear();
    //std::wcout << path << std::endl;
    TagLib::FileRef file(path.c_str());
    if (file.isNull() || file.tag() == nullptr)
    {
        std::cout << "Could not open file.\n";
        return;
    }
    TagLib::Tag* tag = file.tag();
    std::cout << "Title:   " << tag->title().to8Bit() << '\n';
    std::cout << "Artist:  " << tag->artist().to8Bit() << '\n';
    std::cout << "Album:   " << tag->album().to8Bit() << '\n';
    std::cout << "Comment: " << tag->comment().to8Bit() << '\n';
    std::cout << "Genre:   " << tag->genre().to8Bit() << '\n';
    std::cout << "Year:    " << tag->year() << '\n';
    std::cout << "Track:   " << tag->track() << '\n';
}

void write(const std::wstring& path) {
    clear();
    TagLib::FileRef file(path.c_str());
    if (file.isNull() || file.tag() == nullptr)
    {
        std::cout << "Could not open file.\n";
        return;
    }
    TagLib::Tag* tag = file.tag(); 
    std::string title;
    std::string artist;
    std::string album;
    std::string comment;
    std::string genre;
    std::string year;
    std::string track;
    std::wcout << "Currently editing " << path << "." << "\n";
    std::cout << "Title: ";
    std::getline(std::cin, title);
    if (title.empty()) {
        title = "None"; // if input left blank,tag will be "none"
    }
    std::cout << "Artist: ";
    std::getline(std::cin, artist);
    if (artist.empty()) {
        artist = "None";
    }
    std::cout << "Album: ";
    std::getline(std::cin, album);
    if (album.empty()) {
        album = "None";
    }
    std::cout << "Comment: ";
    std::getline(std::cin, comment);
    if (comment.empty()) {
        comment = "None";
    }
    std::cout << "Genre: ";
    std::getline(std::cin, genre);
    if (genre.empty()) {
        genre = "None";
    }
    std::cout << "Year: ";
    std::getline(std::cin, year);
    if (year.empty()) {
        int currentYear = getCurrentYear(); // if input left blank,year will be the current one :)
        year = std::to_string(currentYear);
    }
    std::cout << "Track: ";
    std::getline(std::cin, track);
    if (track.empty()) {
        track = "1";
    }

    tag->setTitle(TagLib::String(title, TagLib::String::UTF8));
    tag->setArtist(TagLib::String(artist, TagLib::String::UTF8));
    tag->setAlbum(TagLib::String(album, TagLib::String::UTF8));
    tag->setComment(TagLib::String(comment, TagLib::String::UTF8));
    tag->setGenre(TagLib::String(genre, TagLib::String::UTF8));
    try
    {
        tag->setYear(std::stoul(year));
        tag->setTrack(std::stoul(track));
    }
    catch (...)
    {
        std::cout << "Invalid year or track.\n";
        return;
    }

    if (file.save()) {
        thankyou();
    }
    else {
        std::cout << "Could not save file";
    }

}

int main()
{
    const wchar_t* logo =
        L"███╗   ███╗██████╗ ██████╗ \n"
        L"████╗ ████║██╔══██╗╚════██╗\n"
        L"██╔████╔██║██████╔╝ █████╔╝\n"
        L"██║╚██╔╝██║██╔═══╝  ╚═══██╗       Ver: Alpha 1.0.0\n"
        L"██║ ╚═╝ ██║██║     ██████╔╝       Dev: Skuno\n"
        L"╚═╝     ╚═╝╚═╝     ╚═════╝ \n";

    DWORD written;
    WriteConsoleW(
        GetStdHandle(STD_OUTPUT_HANDLE),
        logo,
        lstrlenW(logo),
        &written,
        nullptr
    );
    std::wstring path = SelectMP3File();
    if (!path.empty()) {
        size_t pos = path.find_last_of(L"\\/");
        std::wstring fileName;
        if (pos != std::wstring::npos)
            fileName = path.substr(pos + 1);
        else
            fileName = path;

        std::wcout << L"File: " << fileName << std::endl;
        std::cout << "[1] Read" << "\n";
        std::cout << "[2] Write" << "\n";
        std::cout << "[99] Open Repository" << "\n";
        std::cout << "Select: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1) {
            read(path);
        }
        else if(choice == 2) {
            write(path);
        }
        else if (choice == 99) {
            ShellExecute(0, 0, L"https://github.com/skun0/mp3-util", 0, 0, SW_SHOW);
        }
        else {
            std::cout << "Invalid choice.";
        }
    }
    return 0;
}