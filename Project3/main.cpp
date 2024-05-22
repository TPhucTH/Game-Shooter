
#include "main.h"

static void capFrameRate(long* then, float* remainder);
std::vector<const char*> MusicPath = { "music/sontung.mp3","music/TheFatFat.mp3","music_off" };

void for_loop(long then, float remainder) {
    int menuResult = init_menu(); // Hiển thị menu và lấy kết quả
    while (1) {
        prepareScene(); // Chuẩn bị cảnh để vẽ

        doInput(); // Xử lý nhập liệu

        if (menuResult == 0) {
            // Play Game selected
            app.delegate.logic();
            app.delegate.draw();
        }
        else if (menuResult == 1)
        {
            int test = Let_Guide();
            if (test == 0) for_loop(then, remainder);
            else break;
            // print_back();
        }
        else if (menuResult == 2)
        {
            int test = Setup_Sound();
            if (test == 0)
            {
                loadMusic(MusicPath[0]);

                playMusic(1);
            }
            else if (test == 1)
            {
                loadMusic(MusicPath[1]);

                playMusic(1);
            }
            else if (test == 2)
            {
                loadMusic(MusicPath[2]);
            }
            else if (test == 3) for_loop(then, remainder);

            else break;
            // print_back();
        }

        else if (menuResult == 3) {
            // Exit selected
            exit(1);
        }

        presentScene(); // Hiển thị cảnh

        capFrameRate(&then, &remainder); // Giới hạn tốc độ khung hình
    }
}

int main(int argc, char* argv[]) {
    long then;
    float remainder;

    memset(&app, 0, sizeof(App)); // Khởi tạo cấu trúc app
    app.textureTail = &app.textureHead;

    initSDL(); // Khởi tạo SDL
    SDL_ShowCursor(SDL_ENABLE);
    atexit(cleanup); // Đăng ký hàm cleanup để gọi khi thoát chương trình

    start_game();

    initGame(); // Khởi tạo trò chơi
    initTitle(); // Khởi tạo tiêu đề



    then = SDL_GetTicks(); // Lấy thời gian hiện tại
    remainder = 0;

    int menuResult = init_menu(); // Hiển thị menu và lấy kết quả

    if (menuResult == -1) 
    {
        return 0; // Người dùng đã chọn đóng cửa sổ
    }
    for_loop(then, remainder);



    return 0;

}
static void capFrameRate(long* then, float* remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;

    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}
