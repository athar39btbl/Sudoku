#include "EmptyPositionData.h"

DataEmpty::DataEmpty() 
	: m_EmptyList(0)
{}

DataEmpty::~DataEmpty() {
	// �����Ȃ�
}

// �󗓂̏ꏊ����肷�鏈��
void DataEmpty::DeterminePositionEmpty(const SUDOKU_MATRIX& SUDOKU_MATRIX) {
	EMPTY_LIST oEmptyList;
	
	// �󗓂̏ꏊ�����i0�̏��̍s�A��̈ʒu��S�ċL���j
	for (UINT i = 0; i < SUDOKU_MATRIX.rows(); i++) {
		for (UINT j = 0; j < SUDOKU_MATRIX.cols(); j++) {
			if (0 == SUDOKU_MATRIX(i, j)) {
				COORDINATE oCoordinate = std::make_pair(i, j);
				oEmptyList.push_back(oCoordinate);
			}
		}
	}
	// �����o�ϐ��ɃZ�b�g
	SetEmptyList(oEmptyList);
}

// �����o�ϐ��̃Z�b�^
const void DataEmpty::SetEmptyList(const EMPTY_LIST& oEmptyList) {
	m_EmptyList = oEmptyList;

}
// �����o�ϐ��̃Q�b�^
const EMPTY_LIST DataEmpty::GetEmptyList() const {
	return m_EmptyList;
}