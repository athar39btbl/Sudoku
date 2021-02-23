#include <iostream>
#include "BaseSudokuData.h"

#define MAX_LENGTH_STRING (256)
#define INPUTFILENAME (".\\Question.csv")

int main(int argc, char* argv[]) {
	// csvファイルオープン
	char strFileName[MAX_LENGTH_STRING] = "";
	if (argc <= 1) {
		strncpy_s(strFileName, INPUTFILENAME, MAX_LENGTH_STRING);
	}
	else if (2 == argc) {
		strncpy_s(strFileName, argv[1], MAX_LENGTH_STRING);
	}
	else {
		return 1;
	}

	// csvファイルの読み込み
	// 上記を数独データインプットとしてセット
	DataSudoku oSudokuData;
	if (!oSudokuData.LoadSudokuFile(strFileName)) {
		return 1;
	}

	// インプットの結果を出力
	SUDOKU_MATRIX oMatrix = Eigen::MatrixXd::Zero(9, 9);
	oMatrix = oSudokuData.GetSudokuData();
	std::cout << "問題！！" << std::endl;
	std::cout << oMatrix << std::endl;

	// 上記インプットをパラメータとして渡して数独を解く。アウトプットは解き終わった後のインプット
	if (oSudokuData.ExecToSolveSudoku()) {
		return 1;
	}
	
	// インプットの結果を出力
	oMatrix = oSudokuData.GetSudokuData();

	std::cout << "\n\n答え" << std::endl;
	std::cout << oMatrix << std::endl;

	return 0;
}