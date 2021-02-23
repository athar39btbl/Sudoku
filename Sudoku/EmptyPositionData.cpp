#include "EmptyPositionData.h"

DataEmpty::DataEmpty() 
	: m_EmptyList(0)
{}

DataEmpty::~DataEmpty() {
	// 処理なし
}

// 空欄の場所を特定する処理
void DataEmpty::DeterminePositionEmpty(const SUDOKU_MATRIX& SUDOKU_MATRIX) {
	EMPTY_LIST oEmptyList;
	
	// 空欄の場所を特定（0の所の行、列の位置を全て記憶）
	for (UINT i = 0; i < SUDOKU_MATRIX.rows(); i++) {
		for (UINT j = 0; j < SUDOKU_MATRIX.cols(); j++) {
			if (0 == SUDOKU_MATRIX(i, j)) {
				COORDINATE oCoordinate = std::make_pair(i, j);
				oEmptyList.push_back(oCoordinate);
			}
		}
	}
	// メンバ変数にセット
	SetEmptyList(oEmptyList);
}

// メンバ変数のセッタ
const void DataEmpty::SetEmptyList(const EMPTY_LIST& oEmptyList) {
	m_EmptyList = oEmptyList;

}
// メンバ変数のゲッタ
const EMPTY_LIST DataEmpty::GetEmptyList() const {
	return m_EmptyList;
}