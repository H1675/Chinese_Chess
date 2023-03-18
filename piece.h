#ifndef PIECE_H
#define PIECE_H

#include <cmath>
#include <list>
#include <map>
#include <memory>
#include <QDebug>
#define RED_SIDE true
#define BLACK_SIDE false

using Pos = std::pair<int, int>;

class Piece {   //请不要继承QObject
public:
    mutable int x, y;
    //正数
    const enum class PieceType {
        NONE = 0,
        JIANG_SHUAI = 1,
        RED_SHUAI   = 1,    BLACK_JIANG     = -1,
        SHI = 2,
        RED_SHI     = 2,    BLACK_SHI       = -2,
        XIANG = 3,
        RED_XIANG   = 3,    BLACK_XIANG     = -3,
        MA = 4,
        RED_MA      = 4,    BLACK_MA        = -4,
        JU = 5,
        RED_JU      = 5,    BLACK_JU        = -5,
        PAO = 6,
        RED_PAO     = 6,    BLACK_PAO       = -6,
        BING_ZU = 7,
        RED_BING    = 7,    BLACK_ZU        = -7
    } type;
private:
    static const std::map<PieceType, wchar_t> dict;
public:
    static inline wchar_t convert(PieceType type) {
        return dict.at(type);
    }
    inline wchar_t text() const {
        return convert(type);
    }
    inline void move(int x, int y) const {  //请勿修改
        this->x = x;
        this->y = y;
    }
    inline bool side() const {
        return type > PieceType::NONE;
    }
    inline const Pos pos() const {
        return std::make_pair(x, y);
    }
private:
    virtual bool isBasicMove(int x, int y) const = 0;
public:
    bool noThreat(int x, int y) const;
    virtual bool isValidMove(int x, int y) const
    {
        return isBasicMove(x, y) && noThreat(x, y);
    }
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const = 0;
    virtual const std::list<Pos> getPossibleMoves() const = 0;

    Piece(int x, int y, PieceType type) : x(x), y(y), type(type) {}
    virtual ~Piece() {qDebug() << "寄";}  //虚析构
};

inline Piece::PieceType removeSide(Piece::PieceType type) {
    return (Piece::PieceType)std::abs((int)type);
}

inline bool getSide(Piece::PieceType type) {
    return type > Piece::PieceType::NONE;
}


class JIANG_SHUAI : public Piece
{
public:
    bool side;
    JIANG_SHUAI(int x,int y,bool side);
    bool isBasicMove(int x, int y) const;
    const std::list<Pos> getPossibleMoves() const;
    bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    ~JIANG_SHUAI(){ qDebug() << "我寄了1"; };
};

class SHI : public Piece
{
public:
    bool side;
    SHI(int x,int y,bool side);
    bool isBasicMove(int x, int y) const;
    const std::list<Pos> getPossibleMoves() const;
    bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    ~SHI(){ qDebug() << "我寄了2"; };
};

class XIANG : public Piece
{
public:
    bool side;
    XIANG(int x,int y,bool side);
    bool isBasicMove(int x, int y) const;
    const std::list<Pos> getPossibleMoves() const;
    bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    ~XIANG(){ qDebug() << "我寄了3"; };
};

class  MA : public Piece
{
public:
    bool side;
    MA(int x,int y,bool side);
    bool isBasicMove(int x, int y) const;
    const std::list<Pos> getPossibleMoves() const;
    bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    ~MA(){ qDebug() << "我寄了4"; }
};

class JU : public Piece
{
public:
    bool side;
    JU(int x,int y,bool side);
    bool isBasicMove(int x, int y) const;
    const std::list<Pos> getPossibleMoves() const;
    bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    ~JU(){ qDebug() << "我寄了5"; };
};

class PAO : public Piece
{
public:
    bool side;
    PAO(int x,int y,bool side);
    bool isBasicMove(int x, int y) const;
    const std::list<Pos> getPossibleMoves() const;
    bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    ~PAO(){ qDebug() << "我寄了6"; };
};

class BING_ZU : public Piece
{
public:
    bool side;
    BING_ZU(int x,int y,bool side);
    bool isBasicMove(int x, int y) const;
    const std::list<Pos> getPossibleMoves() const;
    bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    ~BING_ZU(){ qDebug() << "我寄了7"; };
};
#endif // PIECE_H
