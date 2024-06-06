#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

//Eventually come back and turn this into bit manipulation with 4 bit numbers: first 3 tell what kind of piece, last tells color


enum Piece_Color { NO_COLOR, WHITE, BLACK };
enum Piece_Type { NO_PIECE, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

struct Chess_Piece {
    Piece_Type type = NO_PIECE;
    Piece_Color color = NO_COLOR;

    Chess_Piece() = default;

    Chess_Piece(Piece_Type t, Piece_Color c) : type(t), color(c) {}

    bool operator==(const Chess_Piece& other) const {
        return type == other.type && color == other.color;
    }
};


// For now only worry about board of size 8X8 figure out how to fix that later
class Chess_Board_Handler {

    std::unordered_map<char, Chess_Piece> fenCharToPieceType;
    std::vector<Chess_Piece> board;

public:
    
    Chess_Board_Handler(std::string fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"){

        fenCharToPieceType = {
            {'k', {KING, BLACK}},
            {'q', {QUEEN, BLACK}},
            {'n', {KNIGHT, BLACK}},
            {'b', {BISHOP, BLACK}},
            {'r', {ROOK, BLACK}},
            {'p', {PAWN, BLACK}},
            {'K', {KING, WHITE}},
            {'Q', {QUEEN, WHITE}},
            {'N', {KNIGHT, WHITE}},
            {'B', {BISHOP, WHITE}},
            {'R', {ROOK, WHITE}},
            {'P', {PAWN, WHITE}},
        };

        initializeBoardFromFenString(fenString);
    }

    //The sizing of creating by fenstring is hardcoded
    void initializeBoardFromFenString(std::string fenString){
        int count = 0;
        board.resize(64);
        for(char c : fenString){
            Chess_Piece piece;
            if(c == '8'){ //HardCoded
                count += 8;
                continue;
            }
            if(c == '/'){
                continue;
            }
            board[count] = fenCharToPieceType[c];
            count++;
        }
    }

    void drawAsciiBoardToConsole(){
        int count = 0;
        for(Chess_Piece piece : board){
            std::cout << "(" << piece.color << " " << piece.type << ") ";
            count++;
            if(count != 0 and count % 8 == 0){ // hardcoded 8
                std::cout << "\n";
            }
        }
    }

    void removePiece(int index){
        board[index].color = NO_COLOR;
        board[index].type = NO_PIECE;
    }

    void replacePiece(int index, Chess_Piece piece){
        board[index] = piece;
    }

    bool isSquareOccupied(int index){
        if(board[index].type == NO_PIECE)
            return false;
        return true;
    }

    Chess_Piece getPieceFromSquare(int index){
        return board[index];
    }

    bool isPieceWhite(int index){
        return board[index].color == WHITE;
    }

    bool isPieceBlack(int index){
        return board[index].color == BLACK;
    }

    bool isIndexOnBoard(int index){
        return (index < (int) board.size() && index > 0);
    }

    /**
     * IF EITHER SQUARE HAS NO COLOR THIS RETURNS FALSE!!!
    */
    bool arePiecesOpositeColors(int indexA, int indexB){
        Chess_Piece pieceA = board[indexA];
        Chess_Piece pieceB = board[indexB];
        if(pieceA.color == NO_COLOR || pieceB.color == NO_COLOR)
            return false;
        if(pieceA.color != pieceB.color)
            return true;
        return false;
    }

    bool hasPawnMoved(int index){
        index++;
        return true;    
    }

    // bool hasKingMoved(int index){

    // }

    // bool hasRookMoved(int index){

    // }

};

class Valid_Move_Checker {

    Chess_Board_Handler * boardHandler;
    public:

    Valid_Move_Checker(Chess_Board_Handler * board_Handler){
        boardHandler = board_Handler;
    }

    //TODO: Need to check whether or not pieces go off the board
    std::vector<int> getValidMovesForPawn(int index){
        std::vector<int> moves;
        if(!boardHandler->isSquareOccupied(index)){
            return moves;
        }
        int indexDiagL;
        int indexDiagR;
        int indexForwardOne;
        int indexForwardTwo;
        if(boardHandler->isPieceBlack(index)){
            indexDiagL = index + 7;
            indexDiagR = index + 9;
            indexForwardOne = index + 8;
            indexForwardTwo = index + 16;
        }
        else{
            indexDiagL = index - 7;
            indexDiagR = index - 9;
            indexForwardOne = index - 8;
            indexForwardTwo = index - 16;
        }
        if(!boardHandler->isSquareOccupied(indexForwardOne)){
            moves.push_back(indexForwardOne);
            if(!boardHandler->isSquareOccupied(indexForwardTwo)){
                moves.push_back(indexForwardTwo);
            }
        }
        if(boardHandler->arePiecesOpositeColors(index, indexDiagL)){
            moves.push_back(indexDiagL);
        }
        if(boardHandler->arePiecesOpositeColors(index, indexDiagR)){
            moves.push_back(indexDiagR);
        }
        return moves;
    }
};

class Chess_Game_Orchestrator {

};

int main(){

    Chess_Board_Handler chess = Chess_Board_Handler();
    chess.drawAsciiBoardToConsole();
    Valid_Move_Checker moveChecker = Valid_Move_Checker(&chess);
    std::cout << "\n\n\n" << std::endl;
    for(int i : moveChecker.getValidMovesForPawn(13)){
        std::cout << i << " ";
    }
    return 0;
}