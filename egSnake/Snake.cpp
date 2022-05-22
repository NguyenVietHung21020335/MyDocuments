#include "Snake.h"
#include "PlayGround.h"

//Khởi tạo rắn
Snake::Snake(PlayGround* playGround)
    : head( new SnakeNode (
              Position(playGround->getWidth() / 2, playGround->getHeight() / 2) //Khởi tạo đốt ở giữa sân chơi
            ) ),
      playGround(playGround),
      direction(Direction::RIGHT),
      cherry(0)
{
    changePlayGroundState(CELL_SNAKE); //Thay đổi trạng thái ở ô này: CELL_SNAKE
}

Snake::~Snake()
{
    //dtor
}

void Snake::processUserInput(UserInput input)
{
    inputQueue.push(input); // Các phần tử được chèn vào và được hủy bỏ từ phía trước.
}

Direction Snake::changeDirection(UserInput input) //Tính hướng đi mới của rắn
{
    switch (input) {
    case KEY_UP:    return direction != DOWN ? UP : direction;
    case KEY_DOWN:  return direction != UP ? DOWN : direction;
    case KEY_LEFT:  return direction != RIGHT ? LEFT : direction;
    case KEY_RIGHT: return direction != LEFT ? RIGHT : direction;
    default:        return direction;
    }
}
//Di chuyển, thay đổi rắn
void Snake::nextStep()
{
    while (!inputQueue.empty()) { //nếu hàng đợi không trống
        UserInput input = inputQueue.front(); //– Sử dụng hàm front();
        inputQueue.pop();   //– Mỗi lần front thì lại thực hiện pop để hàng đợi xóa phần tử đầu tiên, và phần tử tiếp theo sẽ được thế vào vị trí đầu tiên
        Direction newDirection = changeDirection(input);
        if (newDirection != direction) { //nếu có điều hướng mới thì đổi chiều
            direction = newDirection;
            break;
        }
    }

    Position newPosition = head->position.move(direction);  // Tạo biến newPosition để lưu trữ vị trí tiếp theo mà nó tới để ngay dưới đây kiểm tra xem newPosition có hợp lệ k

    if (!checkPosition(newPosition)) {
        playGround->setGameStatus(GAME_LOST);
        return;
    }

    CellType type = playGround->getCellState(newPosition); // Hàm lấy trạng thái ô vuông sắp đi tới

    changePlayGroundState(CELL_EMPTY);
    //Thêm một đốt nếu vừa ăn cherry
    if (cherry > 0) {
        cherry--;
        head = new SnakeNode(newPosition, head);
    } else {  // Trường hợp không ăn, trườn lên phía trước,
        for (SnakeNode* p = head; p != nullptr; p = p->next) {
            std::swap(p->position, newPosition);
        }
    }
    changePlayGroundState(CELL_SNAKE);
    //đánh dấu đã ăn cherry
    if (type == CELL_CHERRY) { //nếu cái vị trí mới chứa cherry thì nó gọi hàm addCherry trong lớp playground để tạo quả mới
        cherry++;
        playGround->addCherry();
    }
}

void Snake::changePlayGroundState(CellType type) //thay đổi sân tùy theo type
{
    for (SnakeNode* p = head; p != nullptr; p = p->next) {
        playGround->changeCellState(p->position, type);
    }
}

bool Snake::checkPosition(Position pos)  //kiểm tra xem có bị ra ngoài sân hay cắn phải thân k
{
    if ( !pos.isInsideBox(0,0,playGround->getWidth(),playGround->getHeight()) )
        return false;

    for (SnakeNode* p = head; p->next != nullptr; p = p->next)
        if (p->position == pos) return false;

    return true;
}
