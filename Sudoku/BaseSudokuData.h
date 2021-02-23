#pragma once
#include <Eigen/Dense>
#include "Utility.h"

#ifndef BASE_SUDOKU_DATA_H
#define BASE_SUDOKU_DATA_H

#define SIZE_BOX   (9)
#define SIZE_BLOCK (3)

typedef Eigen::MatrixXd SUDOKU_MATRIX;

static const std::vector<UINT> oWritableValues = {1,2,3,4,5,6,7,8,9};

// ���ƃf�[�^�N���X
class DataSudoku{
public:
	DataSudoku();
	~DataSudoku();
		
	// ���ƃf�[�^�̎��s����
	int ExecToSolveSudoku();

	// ���ƃf�[�^�̓ǂݍ��ݏ���
	bool LoadSudokuFile(const std::string& strFileName);

	// �����o�ϐ��̃Z�b�^
	const void SetSudokuData(const SUDOKU_MATRIX& oTmp);
	// �����o�ϐ��̃Q�b�^
	const SUDOKU_MATRIX GetSudokuData() const;

protected:

private:
	//�����o�ϐ��A�}�b�g(9x9)�̂�B
	SUDOKU_MATRIX m_SudokuDataSet;
};
#endif //BASE_SUDOKU_DATA_H
