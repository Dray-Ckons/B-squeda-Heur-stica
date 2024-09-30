#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <optional>
#include <vector>

enum MARK {X, O};

class Board
{
    uint16_t board[2]{};
    MARK turn;

    uint16_t oneMask = 0b0000000000000001;
    uint16_t fullMask = 0b1111111111111111;

    int xPieces = 0;
    int oPieces = 0;
    int piecesLimit = 6;
    int phase = 0; // 0 = fase de ubicar fichas, 1 = fase de mover fichas

    bool isLegalMove(int position);  // Verificar si la posición es legal
    bool checkXWin(uint16_t board);  //
    bool checkOWin(uint16_t board);  //

    public:
        Board();  // Constructor prototype
        Board(uint16_t x, uint16_t o, MARK turn);
        ~Board();  // Destructor prototype

        int evaluate(int depth);  // Función de evaluación
        std::vector<int> generateAllLegalMoves();

        uint16_t getXBoard() const;
        uint16_t getOBoard() const;
        MARK getActiveTurn() const;

        void print() const;  // Función para imprimir el tablero en una forma legible
        //bool makeMove(int position);
        std::vector<int> returnPossibleMoves();
        std::vector<int> returnPiecePositions(MARK turn);
        std::vector<int> returnMoves(int fromPosition);

        bool makeMove(int position,int toPosition);  // Función para hacer una jugada
        bool removePiece(int position, MARK turn);// Funcion para remover pieza
        bool movePiece(int from, int to);

        bool checkTriple(uint16_t board);
        bool hasXAlign() const; // Verificar si X ha alineado 3 fichas
        bool haxOAlign() const; // Verificar si O ha alineadi 3 fichas
        bool hasXWon() ;  // Verificar si X ha ganado
        bool hasOWon() ;  // Verificar si O ha ganado
        bool isFull() ;  // Verificar si el tablero está lleno
        bool endGame() ;

        bool operator==(const Board &other) const {
            return turn==other.turn && board[X]==other.board[X] && board[O]==other.board[O];
        }
};

struct BoardHash
{
    std::size_t operator()(Board const& board) const noexcept
    {
        std::size_t h1 = std::hash<uint64_t>{}(board.getXBoard());
        std::size_t h2 = std::hash<uint64_t>{}(board.getOBoard());
        std::size_t h3 = std::hash<bool>{}(board.getActiveTurn() == X);

        // Mezclar usando XOR y desplazamientos
        return h1 ^ (h2 << 1) ^ (h2 >> 1) ^ (h3 << 2) ^ (h3 >> 2);
    }
};



#endif //BOARD_HPP