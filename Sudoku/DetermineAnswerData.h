#pragma once
#include <Eigen/Dense>
#include "EmptyPositionData.h"

#ifndef DETERMINE_ANSWER_DATA_H
#define DETERMINE_ANSWER_DATA_H

typedef std::vector<UINT> ANSWER_LIST;
typedef std::vector<UINT> OVERLAP_LIST;

// �󗓂̌��Ɠ����̃Z�b�g
struct s_DataSetUnit {
	UINT uiRow;						// �󗓉ӏ��̍s
	UINT uiColumn;					// �󗓉ӏ��̗�
	std::vector<UINT> oAnswer;		// �����̌��ƂȂ鐔�̌��

	s_DataSetUnit() 
		: uiRow(0),
		  uiColumn(0)
	{}
};

// �󗓂̌��ƁA�������Z�b�g�œ����Ă���f�[�^�Z�b�g
typedef std::vector<s_DataSetUnit> DATASET;

// �󗓂̌��Ɠ����̃f�[�^�N���X
class DataAnswer {
public:
	DataAnswer();
	~DataAnswer();
	
	// ���������߂�{����
	int SolveSudokuMatrix(SUDOKU_MATRIX& oSudokuMatrix);
	
	// �󗓂̏ꏊ�f�[�^���Ɛ��ƃf�[�^�����A�������f�[�^�N���X�ɓn��
	void CreateDataSet(const SUDOKU_MATRIX& SUDOKU_MATRIX);

	// �����o�ϐ��̃Z�b�^
	const void SetAnswerData(const DATASET& oDataSet);
	
	// �����o�ϐ��̃Q�b�^
	const DATASET GetAnswerData() const;

protected:


private:
	// �����̌�⃊�X�g�̕�������
	bool RestoreAnswerList(s_DataSetUnit& oAnswer, const DATASET& oSudokuMatrix);

	// ��肪����Ƃ���̃`�F�b�N
	bool DoExistOverlapNumber(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiCol);

	// ��肪����s�̃`�F�b�N
	bool DoExistOverlapNumberRow(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow);

	// ��肪�����̃`�F�b�N
	bool DoExistOverlapNumberColumn(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiCol);

	// ��肪���锠�̃`�F�b�N
	bool DoExistOverlapNumberBlock(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiCol);

	// �s�A��A���̒��̐��S�Ă�ΏۂƂ��āA1,2,3,4,5,6,7,8,9�̒��Ŗ�������S�Đ􂢏o������B
	ANSWER_LIST DetermineEnableToAddNumber(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiColumn);
	
	// �s�̒��̐��S�Ă�ΏۂƂ��āA1,2,3,4,5,6,7,8,9�̒��Ŗ�������S�Đ􂢏o������B
	void DetermineEnableToAddNumberRow(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow);
	
	// ��̒��̐��S�Ă�ΏۂƂ��āA1,2,3,4,5,6,7,8,9�̒��Ŗ�������S�Đ􂢏o������B
	void DetermineEnableToAddNumberColumn(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiColumn);
	
	// ���̒��̐��S�Ă�ΏۂƂ��āA1,2,3,4,5,6,7,8,9�̒��Ŗ�������S�Đ􂢏o������B
	void DetermineEnableToAddNumberBlock(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiColumn);

	// �����o�ϐ��@�󗓂̌��Ɠ����̃f�[�^
	DATASET m_oDataSet;
};
#endif //DETERMINE_ANSWER_DATA_H
