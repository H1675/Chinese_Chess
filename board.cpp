#include "algorithms.h"
#include "network.h"
#include<QDebug>

using Type = Piece::PieceType;

Board::Board() {
    //Add your own code below
    //////////////////////////

    //对方移动
    connect(Network::getInstance(),&Network::move,this,&Board::onMove);

    //接收输赢
    connect(Network::getInstance(),&Network::netwin,this,&Board::netwin);

    //////////////////////////
}

void Board::judgeStatus()
{
    checked[!side()] = Algorithms::isCheck(!side());
    checked[side()] = Algorithms::isCheck(side());
    if (Algorithms::isStalemate(side()))
        return emit win(!side());
//    if (Algorithms::isStalemate(!side()))
//        return emit win(side());
}

void Board::onSetup(Cell** cells) {
    for (int i = 0; i < 10 * 9; i++)
    {
        auto& cell = cells[i];
        this->cells.emplace(std::piecewise_construct, std::tuple(cell->x, cell->y), std::tuple(cell));
    }
    std::map<Type, Constructor> factory =
    {
        //请将nullptr替换为'new ClassName(x, y, side)'，请正确设置派生类构造函数参数
        { Type::JIANG_SHUAI, [](int x, int y, bool side)->const Piece* { return new JIANG_SHUAI(x,y,side); } },
        { Type::SHI, [](int x, int y, bool side)->const Piece* { return new SHI(x,y,side); } },
        { Type::XIANG, [](int x, int y, bool side)->const Piece* { return new XIANG(x,y,side); } },
        { Type::MA, [](int x, int y, bool side)->const Piece* { return new MA(x,y,side); } },
        { Type::JU, [](int x, int y, bool side)->const Piece* { return new JU(x,y,side); } },
        { Type::PAO, [](int x, int y, bool side)->const Piece* { return new PAO(x,y,side); } },
        { Type::BING_ZU, [](int x, int y, bool side)->const Piece* { return new BING_ZU(x,y,side); } }
    };
    setPieces(factory);
    for (const auto& [pos, piece] : pieces)
        this->cells[pos]->change(piece);
    if (side()) 
        your_turn = true;

    //Add your own code below
    //////////////////////////



    //////////////////////////
}

void Board::setPieces(const std::map<Type, Constructor> & factory)
{
    static std::list<std::pair<Pos, Type>> piece_list = {
        { { 1, 1 }, Type::RED_JU },
        { { 2, 1 }, Type::RED_MA },
        { { 3, 1 }, Type::RED_XIANG },
        { { 4, 1 }, Type::RED_SHI },
        { { 5, 1 }, Type::RED_SHUAI },
        { { 6, 1 }, Type::RED_SHI },
        { { 7, 1 }, Type::RED_XIANG },
        { { 8, 1 }, Type::RED_MA },
        { { 9, 1 }, Type::RED_JU },
        { { 2, 3 }, Type::RED_PAO },
        { { 8, 3 }, Type::RED_PAO },
        { { 1, 4 }, Type::RED_BING },
        { { 3, 4 }, Type::RED_BING },
        { { 5, 4 }, Type::RED_BING },
        { { 7, 4 }, Type::RED_BING },
        { { 9, 4 }, Type::RED_BING },
        { { 1, 10 }, Type::BLACK_JU },
        { { 2, 10 }, Type::BLACK_MA },
        { { 3, 10 }, Type::BLACK_XIANG },
        { { 4, 10 }, Type::BLACK_SHI },
        { { 5, 10 }, Type::BLACK_JIANG },
        { { 6, 10 }, Type::BLACK_SHI },
        { { 7, 10 }, Type::BLACK_XIANG },
        { { 8, 10 }, Type::BLACK_MA },
        { { 9, 10 }, Type::BLACK_JU },
        { { 2, 8 }, Type::BLACK_PAO },
        { { 8, 8 }, Type::BLACK_PAO },
        { { 1, 7 }, Type::BLACK_ZU },
        { { 3, 7 }, Type::BLACK_ZU },
        { { 5, 7 }, Type::BLACK_ZU },
        { { 7, 7 }, Type::BLACK_ZU },
        { { 9, 7 }, Type::BLACK_ZU }
    };

    for (const auto& [pos, type] : piece_list)
    {
        pieces.emplace(pos, factory.at(removeSide(type))(pos.first, pos.second, side() == getSide(type)));
    }
}

void Board::move(const Pos from, const Pos to)
{
    const auto piece = pieces.at(from);
    piece->move(to.first, to.second);
    auto cell_from = cells.at(from);
    cell_from->change(nullptr);
    auto cell_to = cells.at(to);
    cell_to->change(piece);
    pieces.erase(from);
//    /*  显而易见的胜利条件，但是象棋规则一般是以对方无棋可走为胜利条件
    if (pieces.count(to) && removeSide(pieces.at(to)->type) == Type::JIANG_SHUAI)
        emit win(your_turn ? side() : !side());
//    */
    pieces[to] = piece;
    judgeStatus();
}

void Board::onClick(int x, int y)
{
    std::lock_guard guard(lock);
    static Piece const* selected = nullptr;
    if (!isYourTurn() || isMoved())
        return;
    const auto pos = std::make_pair(x, y);
    if (pieces.count(pos))
    {
        const auto& piece = pieces.at(pos);
        if (piece->side() == side())
        {
            selected = piece;
            cells.at(pos)->select();
            return;
        }
    }
    if (!selected)
        return;
    if (selected->isValidMove(x, y))
    {
        cells.at(pos)->fineMove();

        Network::getInstance()->onMove(std::make_pair(selected->x,selected->y),std::make_pair(x,y));

        move(selected->pos(), pos);
        moved = true;
        your_turn = false;
        selected = nullptr;
    }
    else
    {
        cells.at(pos)->wrongMove();
    }
}

void Board::onMove(const Pos from, const Pos to)
{
    std::lock_guard guard(lock);
    move(from, to);
    your_turn = true;
    moved = false;
}

const std::list<std::pair<Pos, Piece::PieceType>> Board::find(int x, int y, int side) const
{
    //Add your own code here
    //////////////////////////

    std::list<std::pair<Pos, Piece::PieceType>> live_x;
    std::list<std::pair<Pos, Piece::PieceType>> live_y;
    std::list<std::pair<Pos, Piece::PieceType>> lives;

    if(x == -1)
    {
        for(auto& [pos,piece] : pieces)
        {
            live_x.push_back(std::make_pair(pos,piece->type));
        }
    }
    else
    {
        for(auto& [pos,piece] : pieces)
        {
            if(pos.first == x)
            {
                live_x.push_back(std::make_pair(pos,piece->type));
            }
        }
    }

    if(y == -1)
    {
        live_y.swap(live_x);
    }
    else
    {
        for(auto& [pos,type] : live_x)
        {
            if(pos.second == y)
            {

                live_y.push_back(std::make_pair(pos,type));

            }
        }
    }

    if(side == -1)
    {
        return live_y;
    }
    else
    {
        for(auto& [pos,type] : live_y)
        {

            if((int)getSide(type) == side)
            {

                lives.push_back(std::make_pair(pos,type));
            }
        }

    }
    return lives;


    //////////////////////////
}
