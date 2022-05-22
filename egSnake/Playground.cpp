#include <vector>
#include <cassert> //Bắt lỗi
#include <cstdlib>
#include "PlayGround.h"

using namespace std;

//Khởi tạo sân chơi - slide 17 file lec12 Snake course
PlayGround::PlayGround(int width, int height)
    : squares(height, vector<CellType>(width, CELL_EMPTY)), //Khởi tạo ô vuông: dựa vào số dòng, số cột
      snake(this),  // rắn phụ thuộc vào sân chơi, sửa hàm khởi tạo
      status(GAME_RUNNING),
      score(0)
{
    addCherry(); // Thêm 1 quả cherry
}

PlayGround::~PlayGround()
{
    //dtor
}
//Thay đổi trạng thái ô khi khởi tạo/thêm đốt
void PlayGround::changeCellState(Position pos, CellType type)
{
    assert(pos.isInsideBox(0,0,getWidth(),getHeight())); //báo cho ta biết lỗi
    squares[pos.y][pos.x] = type; //Truy nhập x và y từ struct pos -slide 23 file lec12
}

void PlayGround::processUserInput(UserInput input)
{
    snake.processUserInput(input); // Hàm lấy từ PlayGround.h
}

void PlayGround::nextStep()
{
    snake.nextStep();
}
//Cherry mới xuất hiện ngẫu nhiên trong các ô trống(CELL_EMPTY)- slide 44 file lec12
void PlayGround::addCherry()
{
    do {
        Position p(rand()%getWidth(), rand()%getHeight()); // Tạo cherry ngẫu nhiên vs hàm random chiều dài và chiều rộng
        if (getCellState(p) == CELL_EMPTY) {    // Nếu ô vừa random ra trống thì mình cho cherry vào, k thì lại random ra cái khác
            changeCellState(p, CELL_CHERRY);
            break;
        }
    } while (true);
}

void PlayGround::setGameStatus(GameStatus status)
{
    this->status = status;
}
//Lấy vị trí các đốt rắn - slide 54 file lec12
vector<Position> PlayGround::getSnakePositions() const //Dùng hàm const trong PlayGround để bảo vệ dữ liệu về rắn
{
    vector<Position> res;
    for (SnakeNode* p = snake.getHead(); p != nullptr; p = p->next)
        res.push_back(p->position);  //thêm phần tử p->position vào cuối mảng res để có vị trí các đốt rắn khi di chuyển
    return res;
}
