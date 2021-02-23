#include "BaseSudokuData.h"
#include "DetermineAnswerData.h"
#include <iostream>

DataSudoku::DataSudoku() {
	m_SudokuDataSet = SUDOKU_MATRIX::Zero(9, 9);
}

DataSudoku::~DataSudoku() {

}

// ���ƃf�[�^�̎��s����
int DataSudoku::ExecToSolveSudoku() {
	int iRet = 0;

	// �����̌��ƂȂ�f�[�^���ADataAnswer�N���X�ɖ₢���킹
	DataAnswer oDataAnswerSet;
	oDataAnswerSet.CreateDataSet(GetSudokuData());

	// ���Ɖ����̂ɕK�v�ȃf�[�^���Z�b�g
	SUDOKU_MATRIX oDataSudokuForSolve = GetSudokuData();

	// ���Ɖ��������ʂ��i�[
	iRet = oDataAnswerSet.SolveSudokuMatrix(oDataSudokuForSolve);
	
	// ���Ƃ����������ʂ��A�����o�ϐ��ɃZ�b�g
	SetSudokuData(oDataSudokuForSolve);

	return iRet;
}

// ���ƃf�[�^�̓ǂݍ��ݏ���
bool DataSudoku::LoadSudokuFile(const std::string& strFileName) {
	//file open
	std::ifstream oIfs(strFileName.c_str());
	if (!oIfs) {
		return false;
	}

	std::string strBuffer;		// ��s���i�[�p
	SUDOKU_MATRIX oMatrix = Eigen::MatrixXd::Zero(9, 9);;
	UINT uiCnt = 0;

	// �t�@�C���̏���oBuffer�ɋl�ߍ��ށB
	// strBuffer��1�s�i�[����
	while (std::getline(oIfs, strBuffer)) {
		//strBuffer����o�͉\�ɕϊ�����
		std::vector<UINT> oTmp;

		// �J���}��؂育�ƂɃf�[�^���o
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
