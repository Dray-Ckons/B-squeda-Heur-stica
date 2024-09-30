#include <iostream>
#include <optional>
#include "board.hpp"
#include <cstdlib>

Board::Board()
{
    // Bitboards para cada jugador.
    // Usando valores binarios, inicialmente vacíos.
    board[X] = 0b0000000000000000; // Bitboard para X
    board[O] = 0b0000000000000000; // Bitboard para O
    turn = X;  // Comienza jugando X
}

Board::Board(const uint16_t x, const uint16_t o, const MARK turn): board{x, o}, turn(turn) {}

Board::~Board() = default;

uint16_t Board::getXBoard() const { return board[X]; }
uint16_t Board::getOBoard() const { return board[O]; }
MARK Board::getActiveTurn() const { return turn; }

int Board::evaluate(const int depth)
{
    /* Si ha ganado, devolvemos un puntaje positivo
     * Se resta la profundidad para penalizar las victorias que ocurren en niveles más profundos.
     * Esto hace que el algoritmo prefiera ganar más rápidamente si es posible.
     */
    if (hasXWon())
        return (turn == X) ? (17 - depth) : (depth - 17);  // Si 'X' ha ganado, es bueno para el maximizador (X)
    if (hasOWon())
        return (turn == O) ? (17 - depth) : (depth - 17); // Si 'O' ha ganado, es bueno para el minimizador (O)
    if (isFull())
        return 0;  // Empate
    return 0;
}

std::vector<int> Board::generateAllLegalMoves()
{
    std::vector<int> legalMoves;
    if(phase==0){
        for (int i = 0; i < 16; i++){
            if (isLegalMove(i)){
                legalMoves.push_back(i);
            }
        }
    }
    else if(phase==1){
        //std::cout<<turn<<std::endl;
        for(int move : returnPossibleMoves()){
            if(isLegalMove(move)){
                legalMoves.push_back(move);
            }
        }
        /*for(int i = 0; i<16;i++){
            for(int j=0;j<16;j++){
                if(isLegalMove(i,j)){
                    
                    legalMoves.push_back(returnMoves)
                }
            }
        }*/
    }
    return legalMoves;
}

bool Board::isLegalMove(int fromPosition)
{
    if (fromPosition < 0 || fromPosition > 16)  // posicion dentro del rango?
            return false;
    
        if ((board[X] | board[O]) & (oneMask << fromPosition))  // posicion es vacia?
            return false;
        return true;
    
    return false;
}

std::vector<int> Board::returnPossibleMoves(){
    std::vector<int> currentPieces;
    for(int i = 0; i<16;i++){
        if((board[turn]&(oneMask<<i))){
            currentPieces.push_back(i);
        }
    }
    std::vector<int> moves;
    for(int piece : currentPieces){
        for(int move : returnMoves(piece)){
            moves.push_back(move);
        }
    }
    return moves;
}
std::vector<int> Board::returnPiecePositions(){
    std::vector<int> currentPieces;
    for(int i = 0; i<16;i++){
        if((board[turn]&(oneMask<<i))){
            currentPieces.push_back(i);
        }
    }
    return currentPieces;
}

std::vector<int> Board::returnMoves(int fromPosition){
    std::vector<int> moves;
    if(fromPosition==0){
        moves.push_back(1);
        moves.push_back(6);
    }
    else if(fromPosition==1){
        moves.push_back(0);
        moves.push_back(2);
        moves.push_back(4);
    }
    else if(fromPosition==2){
        moves.push_back(1);
        moves.push_back(9);
    }
    else if(fromPosition==3){
        moves.push_back(4);
        moves.push_back(7);
    }
    else if(fromPosition==4){
        moves.push_back(1);
        moves.push_back(3);
        moves.push_back(5);
    }
    else if(fromPosition==5){
        moves.push_back(4);
        moves.push_back(8);
    }
    else if(fromPosition==6){
        moves.push_back(0);
        moves.push_back(7);
        moves.push_back(13);
    }
    else if(fromPosition==7){
        moves.push_back(3);
        moves.push_back(6);
        moves.push_back(10);
    }
    else if(fromPosition==8){
        moves.push_back(5);
        moves.push_back(9);
        moves.push_back(12);
    }
    else if(fromPosition==9){
        moves.push_back(2);
        moves.push_back(8);
        moves.push_back(15);
    }
    else if(fromPosition==10){
        moves.push_back(7);
        moves.push_back(11);
    }
    else if(fromPosition==11){
        moves.push_back(10);
        moves.push_back(12);
        moves.push_back(14);
    }
    else if(fromPosition==12){
        moves.push_back(8);
        moves.push_back(11);
    }
    else if(fromPosition==13){
        moves.push_back(6);
        moves.push_back(14);
    }
    else if(fromPosition==14){
        moves.push_back(11);
        moves.push_back(13);
        moves.push_back(15);
    }
    else if(fromPosition==15){
        moves.push_back(9);
        moves.push_back(14);
    }
    return moves;
}

bool Board::removePiece(int position){
    board[turn] &= ~(oneMask<<position);
    return 0;
}

bool Board::movePiece(int from, int to){
    board[turn] &= ~(oneMask<<from);
    board[turn] |= (oneMask<<to);
    return 0;
}

bool Board::makeMove(const int fromPosition, int toPosition)
{
    if(phase==0 && xPieces==3 && oPieces==3){
        std::cout<<"fase 1"<<std::endl;
        phase=1;
    }
    if (phase==0){
        if (isLegalMove(fromPosition)) {
        board[turn] |= (oneMask << fromPosition);
        if(turn==X){xPieces++;}else{oPieces++;}
        turn = 1-turn == 0 ? X : O;  // cambio de turno
        return true;
        }
    }
    else if(phase==1){
        std::vector<int> legalMoves;
        std::vector<int> destinations;
        for(int from : returnPiecePositions()){
            for(int to : returnMoves(toPosition)){
                if(from==to){
                    std::cout<<from<<"/"<<toPosition<<std::endl;
                    std::cout<<turn<<std::endl;
                    movePiece(from, toPosition);
                    turn = 1-turn == 0 ? X : O;
                    return true;
                }
            }
        }

        /*if (isLegalMove(toPosition)) {
            std::vector<int>positions = returnMoves(toPosition);
            removePiece(positions[std::rand()%(positions.size())]);
            board[turn] |= (oneMask << toPosition);
            turn = 1-turn == 0 ? X : O;
            return true;
        }*/
    }
    return false;
}

bool Board::checkXWin(uint16_t board)
{
    /*if(phase==1 && oPieces<3)
        return true;
    return false;*/
    uint16_t winningBoards[8] = {
        0b1110000000000000,  // fila 1
        0b0001110000000000,  // fila 2
                            // fila 3 es de 2 por un lado y 2 por el otro (no hay conexión)
        0b0000000000111000,  // fila 4
        0b0000000000000111,  // fila 5
        0b1000001000000100,  // columna 1 (exterior)
        0b0001000100100000,  // columna 2 (interior)
                            // columna 3 es de 2 por arriba y 2 por abajo (no hay conexión)
        0b0000010010001000,  // columna 4 (interior)
        0b0010000001000001,  // columna 5 (exterior)
    };
    for (uint16_t wp : winningBoards)
        if ((board & wp) == wp) return true;
    return false;
}

bool Board::checkOWin(uint16_t board)
{
    /*if(phase==1 && xPieces<3)
        return true;
    return false;*/
    uint16_t winningBoards[8] = {
        0b1110000000000000,  // fila 1
        0b0001110000000000,  // fila 2
        // fila 3 es de 2 por un lado y 2 por el otro (no hay conexión)
        0b0000000000111000,  // fila 4
        0b0000000000000111,  // fila 5
        0b1000001000000100,  // columna 1 (exterior)
        0b0001000100100000,  // columna 2 (interior)
        // columna 3 es de 2 por arriba y 2 por abajo (no hay conexión)
        0b0000010010001000,  // columna 4 (interior)
        0b0010000001000001,  // columna 5 (exterior)
    };
    for (uint16_t wp : winningBoards)
        if ((board & wp) == wp) return true;
    return false;
}

bool Board::hasXWon() { return checkXWin(board[X]); }
bool Board::hasOWon() { return checkOWin(board[O]); }
bool Board::isFull()  { return (board[X] | board[O]) == fullMask; }
bool Board::endGame() { return hasXWon() || hasOWon() || isFull(); }

void Board::print() const
{
    //e m p i e z o
    std::cout<<std::endl;
    for (int i = 0; i<2;i++){
        if(board[X] & (oneMask << i))
            std::cout << "X---";
        else if (board[O] & (oneMask << i))
            std::cout << "O---";
        else
            std::cout << "#---";
    }
    if(board[X] & (oneMask << 2))
        std::cout << "X";
    else if (board[O] & (oneMask << 2))
        std::cout << "O";
    else
        std::cout << "#";
    std::cout << std::endl;
    std::cout << "|-";
    for (int i = 3; i<6;i++){
        if(board[X] & (oneMask << i))
            std::cout << "X-";
        else if (board[O] & (oneMask << i))
            std::cout << "O-";
        else
            std::cout << "#-";
    }
    std::cout << "|" << std::endl;
    for (int i = 6; i<8;i++){
        if(board[X] & (oneMask << i))
            std::cout << "X-";
        else if (board[O] & (oneMask << i))
            std::cout << "O-";
        else
            std::cout << "#-";
    }
    std::cout << "-";
    for (int i = 8; i<10;i++){
        if(board[X] & (oneMask << i))
            std::cout << "-X";
        else if (board[O] & (oneMask << i))
            std::cout << "-O";
        else
            std::cout << "-#";
    }
    std::cout << std::endl;
    std::cout << "|-";
    for (int i = 10; i<13;i++){
        if(board[X] & (oneMask << i))
            std::cout << "X-";
        else if (board[O] & (oneMask << i))
            std::cout << "O-";
        else
            std::cout << "#-";
    }
    std::cout << "|" << std::endl;
    for (int i = 13; i<15;i++){
        if(board[X] & (oneMask << i))
            std::cout << "X---";
        else if (board[O] & (oneMask << i))
            std::cout << "O---";
        else
            std::cout << "#---";
    }
    if(board[X] & (oneMask << 15))
        std::cout << "X";
    else if (board[O] & (oneMask << 15))
        std::cout << "O";
    else
        std::cout << "#";
    std::cout << std::endl;
    /*for (int i = 0; i < 9; i++) {  // 9 casillas
        // cada casilla i se compara con ambos tableros para ver si hay una X o O
        if (board[X] & (oneMask << i))
            std::cout << "X ";
        else if (board[O] & (oneMask << i))
            std::cout << "O ";
        else
            std::cout << "- ";
        if (i % 3 == 2) std::cout << std::endl;  // salto de linea cuando completa una fila
    }*/
    std::cout << std::endl;
}
