#include <algorithms.h>

using Type = Piece::PieceType;

const std::map<Type, wchar_t> Piece::dict = {
    { Type::RED_SHUAI, L'帥' },
    { Type::BLACK_JIANG, L'將' },
    { Type::RED_SHI, L'仕' },
    { Type::BLACK_SHI, L'士' },
    { Type::RED_XIANG, L'相' },
    { Type::BLACK_XIANG, L'象' },
    { Type::RED_MA, L'馬' },
    { Type::BLACK_MA, L'馬' },
    { Type::RED_JU, L'車' },
    { Type::BLACK_JU, L'車' },
    { Type::RED_PAO, L'炮' },
    { Type::BLACK_PAO, L'炮' },
    { Type::RED_BING, L'兵' },
    { Type::BLACK_ZU, L'卒' }
};

bool Piece::noThreat(int x, int y) const
{
    std::list<std::pair<Pos,Piece::PieceType>> result_1 = Board::getBoard()->find();
    std::list<std::pair<Pos,Piece::PieceType>> result;

    for(auto& [pos,type] : result_1)
    {
        if(pos.first != x || pos.second != y)
        {
            result.push_back(std::make_pair(pos,type));
        }
    }

    for(auto& [pos,type] : result)
    {
        if(pos.first == this->x && pos.second == this->y)
        {
            pos.first = x;
            pos.second = y;
            break;
        }
    }
    if(Algorithms::isCheck(result,!side()))
    {
        return false;
    }
    return true;
}
//将
JIANG_SHUAI::JIANG_SHUAI(int x,int y,bool side): Piece(x,y,(Type)(side ? 1 : -1))
{
   this->x = x;
   this->y = y;
   this->side = side;
}

bool JIANG_SHUAI::isBasicMove(int x,int y) const
{
    //超出范围
    if(x < 4 || x > 6 || y > 3 || y < 1)
    {
        return false;
    }
    //水平 竖直走一格
    if(this->y == y && std::abs(this->x - x) == 1 && Board::getBoard()->find(x,y,-1).empty())
    {
        return true;
    }
    else if(this->x == x && std::abs(this->y - y) == 1 && Board::getBoard()->find(x,y,-1).empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

const std::list<Pos> JIANG_SHUAI::getPossibleMoves() const
{
    std::list<Pos> a;

    //左右
    if(isValidMove(x - 1,y))
    {
        a.push_back(std::make_pair(x - 1,y));
    }
    if(isValidMove(x + 1,y))
    {
        a.push_back(std::make_pair(x + 1,y));
    }
    //上下
    if(isValidMove(x,y - 1))
    {
        a.push_back(std::make_pair(x,y - 1));
    }
    if(isValidMove(x,y + 1))
    {
        a.push_back(std::make_pair(x,y + 1));
    }
    return a;
}

bool JIANG_SHUAI::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const
{
    int count = 0;
    auto boss = Algorithms::find_boss(pieces,!side);
    for(auto& [pos,type] : pieces)
    {
        if(pos.first == boss.first)
        {
            if(pos.second > this->y && pos.second < boss.second)
            {
                count++;
            }
            else if(pos.second < this->y && pos.second > boss.second)
            {
                count++;
            }
        }
    }
    if(this->x == boss.first && count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//士
SHI::SHI(int x,int y,bool side): Piece(x,y,(Type)(side ? 2 : -2))
{
    this->x = x;
    this->y = y;
    this->side = side;
}

bool SHI::isBasicMove(int x,int y) const
{
    //超出范围
    if(x < 4 || x > 6 || y > 3)
    {
        return false;
    }
    //斜着走一格
    if(std::abs(this->y - y) == 1 && std::abs(this->x - x) == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

const std::list<Pos> SHI::getPossibleMoves() const
{
    std::list<Pos> a;
    //上
    if(isValidMove(x - 1,y + 1))
    {
        a.push_back(std::make_pair(x - 1,y + 1));
    }
    if(isValidMove(x + 1,y + 1))
    {
        a.push_back(std::make_pair(x + 1,y + 1));
    }
    //下
    if(isValidMove(x - 1,y - 1))
    {
        a.push_back(std::make_pair(x - 1,y - 1));
    }
    if(isValidMove(x + 1,y - 1))
    {
        a.push_back(std::make_pair(x + 1,y - 1));
    }
    return a;
}

bool SHI::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const
{
    return false;
}

//象
XIANG::XIANG(int x,int y,bool side): Piece(x,y,(Type)(side ? 3 : -3))
{
    this->x = x;
    this->y = y;
    this->side = side;
}

bool XIANG::isBasicMove(int x, int y) const
{
    if(y > 5 || y < 1)
    {
        return false;
    }
    if(std::abs(this->x - x) == 2 && std::abs(this->y - y) == 2)
    {
        if(Board::getBoard()->find( (this->x+x)/2,(this->y+y)/2,-1).empty())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

const std::list<Pos> XIANG::getPossibleMoves() const
{
    std::list<Pos> a;
    //上
    if(isValidMove(x - 2,y + 2))
    {
        a.push_back(std::make_pair(x - 2,y + 2));
    }
    if(isValidMove(x + 2,y + 2))
    {
        a.push_back(std::make_pair(x + 2,y + 2));
    }
    //下
    if(isValidMove(x - 2,y - 2))
    {
        a.push_back(std::make_pair(x - 2,y - 2));
    }
    if(isValidMove(x + 2,y - 2))
    {
        a.push_back(std::make_pair(x + 2,y - 2));
    }
    return a;
}

bool XIANG::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const
{
    return false;
}

//马
MA::MA(int x,int y,bool side): Piece(x,y,(Type)(side ? 4 : -4))
{
    this->x = x;
    this->y = y;
    this->side = side;
}

bool MA::isBasicMove(int x, int y) const
{
    if(x < 1 || x > 9 || y < 1 || y > 10)
    {
        return false;
    }
    if((std::abs(this->x - x) == 1 && std::abs(this->y - y) == 2) ||
            (std::abs(this->x - x) == 2 && std::abs(this->y - y) == 1))
    {
        if(std::abs(this->x - x) == 1)
        {
            if(Board::getBoard()->find(this->x,(this->y+y)/2,-1).empty())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(Board::getBoard()->find((this->x+x)/2,this->y,-1).empty())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
}

const std::list<Pos> MA::getPossibleMoves() const
{
    std::list<Pos> a;
    //竖着
    if(isValidMove(x-1,y+2))
    {
        a.push_back(std::make_pair(x-1,y+2));
    }
    if(isValidMove(x+1,y+2))
    {
        a.push_back(std::make_pair(x+1,y+2));
    }
    if(isValidMove(x-1,y-2))
    {
        a.push_back(std::make_pair(x-1,y-2));
    }
    if(isValidMove(x+1,y-2))
    {
        a.push_back(std::make_pair(x+1,y-2));
    }
    //横着
    if(isValidMove(x-2,y+1))
    {
        a.push_back(std::make_pair(x-2,y+1));
    }
    if(isValidMove(x+2,y+1))
    {
        a.push_back(std::make_pair(x+2,y+1));
    }
    if(isValidMove(x-2,y-1))
    {
        a.push_back(std::make_pair(x-2,y-1));
    }
    if(isValidMove(x+2,y-1))
    {
        a.push_back(std::make_pair(x+2,y-1));
    }
    return a;
}

bool MA::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const
{
    auto boss = Algorithms::find_boss(pieces,!side);
    if(std::abs(this->x - boss.first) == 1 && std::abs(this->y - boss.second) == 2)
    {
        for(auto& [pos,type] : pieces)
        {
            if(pos.first == this->x && pos.second == (boss.second + this->y)/2)
            {
                return false;
            }
        }
        return true;
    }
    else if(std::abs(this->x - boss.first) == 2 && std::abs(this->y - boss.second) == 1)
    {
        for(auto& [pos,type] : pieces)
        {
            if(pos.second == this->y && pos.first == (boss.first + this->x)/2)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

//车
JU::JU(int x,int y,bool side): Piece(x,y,(Type)(side ? 5 : -5))
{
    this->x = x;
    this->y = y;
    this->side = side;
}

bool JU::isBasicMove(int x, int y) const
{
    if(this->x == x || this->y == y)
    {
        if(Algorithms::find_from_to(std::make_pair(this->x,this->y),std::make_pair(x,y)).empty())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

const std::list<Pos> JU::getPossibleMoves() const
{
    std::list<Pos> a;
    for(int i = 1;i <= 10;i++)
    {
        if(isValidMove(x,i) && i != y)
        {
            a.push_back(std::make_pair(x,i));
        }
    }

    for(int i = 1;i <= 9;i++)
    {
        if(isValidMove(i,y) && i != x)
        {
            a.push_back(std::make_pair(i,y));
        }
    }
    return a;
}

bool JU::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const
{
    auto boss = Algorithms::find_boss(pieces,!side);
    if(boss.first == this->x)
    {
        for(auto& [pos,type] : pieces)
        {
            if(pos.first == this->x)
            {
                if(pos.second < boss.second && pos.second > this->y)
                {
                    return false;
                }
                else if(pos.second > boss.second && pos.second < this->y)
                {
                    return false;
                }
            }
        }
        return true;
    }
    else if(boss.second == this->y)
    {
        for(auto& [pos,type] : pieces)
        {
            if(pos.second == this->y)
            {
                if(pos.first < boss.first && pos.first > this->x)
                {
                    return false;
                }
                else if(pos.first > boss.first && pos.first < this->x)
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

//炮
PAO::PAO(int x,int y,bool side): Piece(x,y,(Type)(side ? 6 : -6))
{
    this->x = x;
    this->y = y;
    this->side = side;
}

bool PAO::isBasicMove(int x, int y) const
{
    if(this->x == x || this->y == y)
    {
        if(Board::getBoard()->find(x,y,!side).size() == 1 && Algorithms::find_from_to(pos(),std::make_pair(x,y)).size() == 2)
        {
            return true;
        }
        else if(Board::getBoard()->find(x,y,!side).empty() && Algorithms::find_from_to(pos(),std::make_pair(x,y)).empty())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

const std::list<Pos> PAO::getPossibleMoves() const
{
    std::list<Pos> a;
    for(int i = 1;i <= 10;i++)
    {
        if(isValidMove(x,i) && i != y)
        {
            a.push_back(std::make_pair(x,i));
        }
    }

    for(int i = 1;i <= 9;i++)
    {
        if(isValidMove(i,y) && i != x)
        {
            a.push_back(std::make_pair(i,y));
        }
    }
    return a;
}

bool PAO::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const
{
    int count = 0;
    auto boss = Algorithms::find_boss(pieces,!side);
    if(boss.first == this->x)
    {
        for(auto& [pos,type] : pieces)
        {
            if(pos.first == this->x)
            {
                if(pos.second < boss.second && pos.second > this->y)
                {
                    count++;
                }
                else if(pos.second > boss.second && pos.second < this->y)
                {
                    count++;
                }
            }
        }
        if(count == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(boss.second == this->y)
    {
        for(auto& [pos,type] : pieces)
        {
            if(pos.second == this->y)
            {
                if(pos.first < boss.first && pos.first > this->x)
                {
                    count++;
                }
                else if(pos.first > boss.first && pos.first < this->x)
                {
                    count++;
                }
            }
        }
        if(count == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

//兵
BING_ZU::BING_ZU(int x,int y,bool side): Piece(x,y,(Type)(side ? 7 : -7))
{
    this->x = x;
    this->y = y;
    this->side = side;
}

bool BING_ZU::isBasicMove(int x, int y) const
{
    if(this->y < 6)
    {
        if(y - this->y == 1 && this->x == x)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(this->y == y && std::abs(this->x - x) == 1)
        {
            return true;
        }
        else if(this->x == x && y - this->y == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

const std::list<Pos> BING_ZU::getPossibleMoves() const
{
    std::list<Pos> a;
//    if(y < 6)
//    {
//        a.push_back(std::make_pair(x,y+1));
//        qDebug() << x << "," << y+1;
//    }
//    else if(y >= 6)
//    {
//        if(isValidMove(x-1,y))
//        {
//            a.push_back(std::make_pair(x-1,y));
//            qDebug() << x-1 << "," << y;
//        }
//        if(isValidMove(x+1,y))
//        {
//            a.push_back(std::make_pair(x+1,y));
//            qDebug() << x+1 << "," << y;
//        }
//        if(isValidMove(x,y+1))
//        {
//            a.push_back(std::make_pair(x,y+1));
//            qDebug() << x << "," << y+1;
//        }
//    }
    if(isValidMove(x-1,y))
    {
        a.push_back(std::make_pair(x-1,y));
    }
    if(isValidMove(x+1,y))
    {
        a.push_back(std::make_pair(x+1,y));
    }
    if(isValidMove(x,y+1))
    {
        a.push_back(std::make_pair(x,y+1));
    }
    return a;
}

bool BING_ZU::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const
{
    auto boss = Algorithms::find_boss(pieces,!side);
    if(isBasicMove(boss.first,boss.second))
    {
        return true;
    }
    else
    {
        return false;
    }
}
