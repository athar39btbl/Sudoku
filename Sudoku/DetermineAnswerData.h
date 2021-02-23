#pragma once
#include <Eigen/Dense>
#include "EmptyPositionData.h"

#ifndef DETERMINE_ANSWER_DATA_H
#define DETERMINE_ANSWER_DATA_H

typedef std::vector<UINT> ANSWER_LIST;
typedef std::vector<UINT> OVERLAP_LIST;

// 空欄の個所と答えのセット
struct s_DataSetUnit {
	UINT uiRow;						// 空欄箇所の行
	UINT uiColumn;					// 空欄箇所の列
	std::vector<UINT> oAnswer;		// 答えの候補となる数の候補

	s_DataSetUnit() 
		: uiRow(0),
		  uiColumn(0)
	{}
};

// 空欄の個所と、答えがセットで入っているデータセット
typedef std::vector<s_DataSetUnit> DATASET;

// 空欄の個所と答えのデータクラス
class DataAnswer {
public:
	DataAnswer();
	~DataAnswer();
	
	// 答えを求める本処理
	int SolveSudokuMatrix(SUDOKU_MATRIX& oSudokuMatrix);
	
	// 空欄の場所データ情報と数独データ情報を、答え候補データクラスに渡す
	void CreateDataSet(const SUDOKU_MATRIX& SUDOKU_MATRIX);

	// メンバ変数のセッタ
	const void SetAnswerData(const DATASET& oDataSet);
	
	// メンバ変数のゲッタ
	const DATASET GetAnswerData() const;

protected:


private:
	// 答えの候補リストの復元処理
	bool RestoreAnswerList(s_DataSetUnit& oAnswer, const DATASET& oSudokuMatrix);

	// 被りがあるところのチェック
	bool DoExistOverlapNumber(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiCol);

	// 被りがある行のチェック
	bool DoExistOverlapNumberRow(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow);

	// 被りがある列のチェック
	bool DoExistOverlapNumberColumn(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiCol);

	// 被りがある箱のチェック
	bool DoExistOverlapNumberBlock(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiCol);

	// 行、列、箱の中の数全てを対象として、1,2,3,4,5,6,7,8,9の中で無い数を全て洗い出させる。
	ANSWER_LIST DetermineEnableToAddNumber(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiColumn);
	
	// 行の中の数全てを対象として、1,2,3,4,5,6,7,8,9の中で無い数を全て洗い出させる。
	void DetermineEnableToAddNumberRow(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow);
	
	// 列の中の数全てを対象として、1,2,3,4,5,6,7,8,9の中で無い数を全て洗い出させる。
	void DetermineEnableToAddNumberColumn(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiColumn);
	
	// 箱の中の数全てを対象として、1,2,3,4,5,6,7,8,9の中で無い数を全て洗い出させる。
	void DetermineEnableToAddNumberBlock(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiColumn);

	// メンバ変数　空欄の個所と答えのデータ
	DATASET m_oDataSet;
};
#endif //DETERMINE_ANSWER_DATA_H
