#pragma once
#include <vector>
#include <Algorithm>
#include <Eigen/Dense>
#include "BaseSudokuData.h"

#ifndef EMPTY_POSITION_DATA_H
#define EMPTY_POSITION_DATA_H

typedef std::pair<UINT, UINT> COORDINATE;
typedef std::vector<COORDINATE> EMPTY_LIST;

// 空欄箇所のデータクラス
class DataEmpty {
public:
	DataEmpty();
	~DataEmpty();

	// 空欄の場所を特定する処理
	void DeterminePositionEmpty(const SUDOKU_MATRIX& SUDOKU_MATRIX);
	// メンバ変数のセッタ
	const void SetEmptyList(const EMPTY_LIST& oEmptyList);
	// メンバ変数のゲッタ
	const EMPTY_LIST GetEmptyList() const;

protected:
private:

	// メンバ変数　空欄の場所リスト
	EMPTY_LIST m_EmptyList;
};
#endif //EMPTY_POSITION_DATA_H
