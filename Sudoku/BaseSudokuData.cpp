#include "BaseSudokuData.h"
#include "DetermineAnswerData.h"
#include <iostream>

DataSudoku::DataSudoku() {
	m_SudokuDataSet = SUDOKU_MATRIX::Zero(9, 9);
}

DataSudoku::~DataSudoku() {

}

// 数独データの実行処理
int DataSudoku::ExecToSolveSudoku() {
	int iRet = 0;

	// 答えの候補となるデータを、DataAnswerクラスに問い合わせ
	DataAnswer oDataAnswerSet;
	oDataAnswerSet.CreateDataSet(GetSudokuData());

	// 数独解くのに必要なデータをセット
	SUDOKU_MATRIX oDataSudokuForSolve = GetSudokuData();

	// 数独解いた結果を格納
	iRet = oDataAnswerSet.SolveSudokuMatrix(oDataSudokuForSolve);
	
	// 数独を解いた結果を、メンバ変数にセット
	SetSudokuData(oDataSudokuForSolve);

	return iRet;
}

// 数独データの読み込み処理
bool DataSudoku::LoadSudokuFile(const std::string& strFileName) {
	//file open
	std::ifstream oIfs(strFileName.c_str());
	if (!oIfs) {
		return false;
	}

	std::string strBuffer;		// 一行ずつ格納用
	SUDOKU_MATRIX oMatrix = Eigen::MatrixXd::Zero(9, 9);;
	UINT uiCnt = 0;

	// ファイルの情報をoBufferに詰め込む。
	// strBufferに1行格納する
	while (std::getline(oIfs, strBuffer)) {
		//strBufferを入出力可能に変換する
		std::vector<UINT> oTmp;

		// カンマ区切りごとにデータ抽出
		Tokenize(oTmp, strBuffer);
		for (UINT uiCol = 0; uiCol < oMatrix.cols(); uiCol++) {
			oMatrix(uiCnt, uiCol) = oTmp.at(uiCol);
		}
		uiCnt++;
	}
	oIfs.close();
	SetSudokuData(oMatrix);
	
	return true;
}

const void DataSudoku::SetSudokuData(const SUDOKU_MATRIX& oTmp) {
	m_SudokuDataSet = oTmp;
}

const SUDOKU_MATRIX DataSudoku::GetSudokuData() const {
	return m_SudokuDataSet;
}
