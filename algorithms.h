#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "board.h"
#include<QDebug>

//工具类
class Algorithms {
public:
    //判断僵局(无子可走)
    static bool isStalemate(bool side) {
        auto result = Board::getBoard()->find(-1, -1, side);

        for(const auto& [pos,type] : result)
        {
            auto piece = Board::getBoard()->pieces.at(pos);
            qDebug() << (int)piece->type << piece->getPossibleMoves().size();
        }

        for (const auto& [pos, type] : result)
        {
            auto piece = Board::getBoard()->pieces.at(pos);
            if (!piece->getPossibleMoves().empty())
                return false;
        }
        return true;
    }

    //Piece::noThreat和Algorithms::isCheck的推荐辅助函数
    static inline bool isCheck(const std::list<std::pair<Pos, Piece::PieceType>>& pieces, bool side)
    {
        for(auto& [pos,type] : pieces)
        {
            bool Type = (int)type > 0;
            if( Type == side )
            {
                auto piece = Board::getBoard()->pieces.at(pos);
                if(piece->isThreat(pieces))
                {
                    return true;
                }
            }
        }
        return false;
    }

    static bool isCheck(bool side)
    {
        return isCheck(Board::getBoard()->find(), side);
    }

    static Pos find_boss(const std::list<std::pair<Pos, Piece::PieceType>>& pieces,bool side)
    {
        for(auto& [pos,type] : pieces)
        {
            if(side)
            {
                if((int)type == 1)
                {
                    return pos;
                }
            }
            else
            {
                if((int)type == -1)
                {
                    return pos;
                }
            }
        }
        return std::make_pair(0,0);
    }

    static std::list<std::pair<Pos, Piece::PieceType>> find_from_to(Pos from,Pos to)
    {
        std::list<std::pair<Pos, Piece::PieceType>> number;
        auto& result = Board::getBoard()->find();
        if(from.first == to.first)
        {
            if(from.second > to.second)
            {
                for(auto& [pos,type] : result)
                {
                    if(pos.first == from.first && pos.second >= to.second && pos.second < from.second)
                    {
                        number.push_back(std::make_pair(pos,type));
                    }
                }
            }
            else
            {
                for(auto& [pos,type] : result)
                {
                    if(pos.first == from.first && pos.second <= to.second && pos.second > from.second)
                    {
                        number.push_back(std::make_pair(pos,type));
                    }
                }
            }
        }
        else
        {
            if(from.first > to.first)
            {
                for(auto& [pos,type] : result)
                {
                    if(pos.second == from.second && pos.first >= to.first && pos.first < from.first)
                    {
                        number.push_back(std::make_pair(pos,type));
                    }
                }
            }
            else
            {
                for(auto& [pos,type] : result)
                {
                    if(pos.second == from.second && pos.first <= to.first && pos.first > from.first)
                    {
                        number.push_back(std::make_pair(pos,type));
                    }
                }
            }
        }
        return number;
    }
};

#endif // ALGORITHMS_H
