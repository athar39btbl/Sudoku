#pragma once
#include <Eigen/Dense>
#include "Utility.h"

#ifndef BASE_SUDOKU_DATA_H
#define BASE_SUDOKU_DATA_H

#define SIZE_BOX   (9)
#define SIZE_BLOCK (3)

typedef Eigen::MatrixXd SUDOKU_MATRIX;

static const std::vector<UINT> oWritableValues = {1,2,3,4,5,6,7,8,9};

// 数独データクラス
class DataSudoku{
public:
	DataSudoku();
	~DataSudoku();
		
	// 数独データの実行処理
	int ExecToSolveSudoku();

	// 数独データの読み込み処理
	bool LoadSudokuFile(const std::string& strFileName);

	// メンバ変数のセッタ
	const void SetSudokuData(const SUDOKU_MATRIX& oTmp);
	// メンバ変数のゲッタ
	const SUDOKU_MATRIX GetSudokuData() const;

protected:

private:
	//メンバ変数、マット(9x9)のやつ。
	SUDOKU_MATRIX m_SudokuDataSet;
};
#endif //BASE_SUDOKU_DATA_H
