#include "DetermineAnswerData.h"

DataAnswer::DataAnswer() 
	: m_oDataSet(0)
{}

DataAnswer::~DataAnswer() {
	// �����Ȃ�
}

// ���������߂�{����
int DataAnswer::SolveSudokuMatrix(SUDOKU_MATRIX& oSudokuMatrix) {
	DATASET oDataSet                 = GetAnswerData();
	DATASET oDataSetBack             = oDataSet;
	SUDOKU_MATRIX oDataSudokuForSolve = oSudokuMatrix;

	auto oItDataSet = oDataSet.begin();
	// ��LDATASET�̐��������[�v����
	while (oItDataSet != oDataSet.end()) {
		UINT uiRow = oItDataSet->uiRow;
		UINT uiColumn = oItDataSet->uiColumn; 

		// SUDOKU_MATRIX�������O�ɁA�����̌�₪�܂�����̂��m�F
		if ((oItDataSet->oAnswer).empty()) {
			// �폜�������������ɖ߂��B
			if (RestoreAnswerList(*oItDataSet, oDataSetBack)) {
				// SUDOKU_MATRIX�������O�̏�Ԃɖ߂�
				oDataSudokuForSolve(uiRow, uiColumn) = 0;
			}

			// �����̂ڂ��Ĉ�ԏ��ߖ��߂��Ă��܂�����A�����Ȃ��Ƃ������ƂȂ̂ŏ����I����
			if (oDataSet.begin() == oItDataSet) {
				return 1;
			}
			
			// �C�e���[�^����O�ɖ߂��ď����𑱂���
			--oItDataSet;
			continue;
		}

		// �����̌��̐��������[�v�𑱂���
		auto oItAnswer = (oItDataSet->oAnswer).begin();
		while (oItAnswer != (oItDataSet->oAnswer).end()) {
			oDataSudokuForSolve(uiRow, uiColumn) = *oItAnswer;
			
			// ��肪����̂��m�F
			bool bRet = DoExistOverlapNumber(oDataSudokuForSolve, uiRow, uiColumn);
			
			// ��Ŏg�����������폜���āA�����̌�⃊�X�g�p�̃C�e���[�^����i�߂�
			oItAnswer = (oItDataSet->oAnswer).erase(oItAnswer);

			if (!bRet) {
				// ��肪�Ȃ���΂������g�����������폜���āA���̃f�[�^�Z�b�g������
				++oItDataSet;
				break;
			}

			// oItDataSet ���̓������������s�ł��Ȃ��Ȃ�E�E�E
			if ((oItDataSet->oAnswer).empty()) {
				// �폜�������������ɖ߂��B
				if (RestoreAnswerList(*oItDataSet, oDataSetBack)) {
					// SUDOKU_MATRIX�������O�̏�Ԃɖ߂�
					oDataSudokuForSolve(uiRow, uiColumn) = 0;
				}

				// �����̂ڂ��Ĉ�ԏ��ߖ��߂��Ă��܂�����A�����Ȃ��Ƃ������ƂȂ̂ŏ����I����
				if (oDataSet.begin() == oItDataSet) {
					return 1;
				}
				// �C�e���[�^����O�ɖ߂��ď����𑱂���
				--oItDataSet;
				break; 
			}
			// oItDataSet ���̓������܂��c���Ăď����ł���Ȃ�E�E�E
			// oItDataSet ���́@���̓����ɐi��
			else {
				continue;
			}
		}
	}

	oSudokuMatrix = oDataSudokuForSolve;
	return 0;
}


// �󗓂̏ꏊ�f�[�^���Ɛ��ƃf�[�^�����A�������f�[�^�N���X�ɓn��
void DataAnswer::CreateDataSet(const SUDOKU_MATRIX& oSudokuMatrix) {
	// �󗓂̏ꏊ�𒲂ׂāA���ʂ��擾�B
	DataEmpty oDataEmpty;
	oDataEmpty.DeterminePositionEmpty(oSudokuMatrix);
	
	DATASET oDataSet;

	// �󗓂̏ꏊ���擾
	EMPTY_LIST oEmptyList = oDataEmpty.GetEmptyList();
	auto oItEmptyList = oEmptyList.begin();
	// ���ƂȂ鐔�ƌ��ѕt���āA���X�g�����
	for (; oItEmptyList != oEmptyList.end(); ++oItEmptyList) {
		// �s�A��A���̒��̐��S�Ă�ΏۂƂ��āA1,2,3,4,5,6,7,8,9�̒��Ŗ�������S�Đ􂢏o���B
		s_DataSetUnit oDataSetUnit;
		oDataSetUnit.uiRow    = oItEmptyList->first;
		oDataSetUnit.uiColumn = oItEmptyList->second;
		oDataSetUnit.oAnswer  = DetermineEnableToAddNumber(oSudokuMatrix, oDataSetUnit.uiRow, oDataSetUnit.uiColumn);

		oDataSet.push_back(oDataSetUnit);
	}

	// �����������o����Ƃ��납��������߂ɁAoAnswer�̃T�C�Y�����������Ƀ\�[�g
	std::sort(oDataSet.begin(), oDataSet.end(),
		[](const s_DataSetUnit& x, const s_DataSetUnit& y) 
			{ return x.oAnswer.size() < y.oAnswer.size(); }
		);

	// �����o�ϐ��ɃZ�b�g
	SetAnswerData(oDataSet);
}

// �����o�ϐ��̃Z�b�^
const void DataAnswer::SetAnswerData(const DATASET& oDataSet) {
	m_oDataSet = oDataSet;
}
// �����o�ϐ��̃Q�b�^
const DATASET DataAnswer::GetAnswerData() const {
	return m_oDataSet;
}

// �����̌�⃊�X�g�̕�������
bool DataAnswer::RestoreAnswerList(s_DataSetUnit& oDataSetUnit, const DATASET& oSudokuMatrix) {
	
	auto oItDataSetBack = oSudokuMatrix.begin();
	for (; oItDataSetBack != oSudokuMatrix.end(); ++oItDataSetBack) {
		if ((oItDataSetBack->uiRow == oDataSetUnit.uiRow) && (oItDataSetBack->uiColumn == oDataSetUnit.uiColumn)) {
			// �����O�̓����̃��X�g�ɖ߂����߂ɑ������
			oDataSetUnit.oAnswer = oItDataSetBack->oAnswer;
			return true;
		}
	}

	return false;
}

// ��肪����Ƃ���̃`�F�b�N
// ���݂��Ă���true
// ���݂��ĂȂ�������false
bool DataAnswer::DoExistOverlapNumber(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiCol) {
	bool bRet = true;

	if (!DoExistOverlapNumberRow(oSudokuMatrix, uiRow)) {
		return bRet;
	}

	if (!DoExistOverlapNumberColumn(oSudokuMatrix, uiCol)) {
		return bRet;
	}

	if (!DoExistOverlapNumberBlock(oSudokuMatrix, uiRow, uiCol)) {
		return bRet;
	}

	bRet = false;
	return bRet;
}

// ��肪����s�̃`�F�b�N
bool DataAnswer::DoExistOverlapNumberRow(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow) {
	OVERLAP_LIST oTmpRow;
	oTmpRow.reserve(oSudokuMatrix.rows());
	// row�œ��������Ȃ������肷��
	for (UINT i = 0; i < oSudokuMatrix.rows(); i++) {
		if (0 == oSudokuMatrix(uiRow, i)) {
			continue;
		}
		oTmpRow.push_back(oSudokuMatrix(uiRow, i));
	}

	// ��肪���邩�m�F
	std::sort(oTmpRow.begin(), oTmpRow.end());
	auto oItRowUnique = std::unique(oTmpRow.begin(), oTmpRow.end());
	if (oItRowUnique == oTmpRow.end()) {
		return true;	// ���݂��Ă�
	}
	return false;
}

// ��肪�����̃`�F�b�N
bool DataAnswer::DoExistOverlapNumberColumn(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiCol) {
	OVERLAP_LIST oTmpRow;
	oTmpRow.reserve(oSudokuMatrix.cols());

	// col�œ��������Ȃ������肷��
	for (UINT i = 0; i < oSudokuMatrix.rows(); i++) {
		if (0 == oSudokuMatrix(i, uiCol)) {
			continue;
		}
		oTmpRow.push_back(oSudokuMatrix(i, uiCol));
	}

	// ��肪���邩�m�F
	std::sort(oTmpRow.begin(), oTmpRow.end());
	auto oItRowUnique = std::unique(oTmpRow.begin(), oTmpRow.end());
	if (oItRowUnique == oTmpRow.end()) {
		return true;	// ���݂��Ă�
	}
	return false;
}

// ��肪���锠�̃`�F�b�N
bool DataAnswer::DoExistOverlapNumberBlock(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiCol) {
	OVERLAP_LIST oTmpBlock;
	oTmpBlock.reserve(oSudokuMatrix.rows());

	UINT uiStartRow = uiRow / SIZE_BLOCK * SIZE_BLOCK;
	UINT uiStartCol = uiCol / SIZE_BLOCK * SIZE_BLOCK;
	Eigen::MatrixXd oTmpMatrix = oSudokuMatrix.block<SIZE_BLOCK, SIZE_BLOCK>(uiStartRow, uiStartCol);

	// block�œ��������Ȃ������肷��
	for (UINT i = 0; i < oSudokuMatrix.cols(); i++) {
		if (0 == oTmpMatrix(i)) {
			continue;
		}
		oTmpBlock.push_back(oTmpMatrix(i));
	}

	// ��肪���邩�m�F
	std::sort(oTmpBlock.begin(), oTmpBlock.end());
	auto oItRowUnique = std::unique(oTmpBlock.begin(), oTmpBlock.end());
	if (oItRowUnique == oTmpBlock.end()) {
		return true;	// ���݂��Ă�
	}
	return false;
}


// �������f�[�^�N���X���ŁA�����̌������߂�B
ANSWER_LIST DataAnswer::DetermineEnableToAddNumber(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiColumn) {
	ANSWER_LIST oData = oWritableValues;

	DetermineEnableToAddNumberRow(oData, oSudokuMatrix, uiRow);

	DetermineEnableToAddNumberColumn(oData, oSudokuMatrix, uiColumn);

	DetermineEnableToAddNumberBlock(oData, oSudokuMatrix, uiRow, uiColumn);

	return oData;
}

// �������f�[�^�N���X���ŁA�s�̒��œ����̌������߂�B
void DataAnswer::DetermineEnableToAddNumberRow(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow) {
	ANSWER_LIST oTmpRow;
	// row�œ��������Ȃ������肷��
	for (UINT i = 0; i < oSudokuMatrix.rows(); i++) {
		if (0 == oSudokuMatrix(uiRow, i)) {
			continue;
		}
		oTmpRow.push_back(oSudokuMatrix(uiRow, i));
	}

	auto oItTmpRow = oTmpRow.begin();
	for (; oItTmpRow != oTmpRow.end(); ++oItTmpRow) {
		oData.erase(std::remove(oData.begin(), oData.end(), *oItTmpRow), oData.end());
	}
}

// �������f�[�^�N���X���ŁA��̒��œ����̌������߂�B
void DataAnswer::DetermineEnableToAddNumberColumn(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiColumn) {
	ANSWER_LIST oTmpCol;
	// column�œ��������Ȃ������肷��
	for (UINT i = 0; i < oSudokuMatrix.cols(); i++) {
		if (0 == oSudokuMatrix(i, uiColumn)) {
			continue;
		}
		oTmpCol.push_back(oSudokuMatrix(i, uiColumn));
	}

	auto oItTmpCol = oTmpCol.begin();
	for (; oItTmpCol != oTmpCol.end(); ++oItTmpCol) {
		oData.erase(std::remove(oData.begin(), oData.end(), *oItTmpCol), oData.end());
	}
}

// �������f�[�^�N���X���ŁA�u���b�N�̒��œ����̌������߂�B
void DataAnswer::DetermineEnableToAddNumberBlock(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiColumn) {
	ANSWER_LIST oTmpBlock;

	// �u���b�N�̍�����W�����߂�
	UINT uiOrgRow = uiRow / SIZE_BLOCK * SIZE_BLOCK;
	UINT uiOrgCol = uiColumn / SIZE_BLOCK * SIZE_BLOCK;
	Eigen::MatrixXd oTmpMatrix = oSudokuMatrix.block<SIZE_BLOCK, SIZE_BLOCK>(uiOrgRow, uiOrgCol);

	// block�œ��������Ȃ������肷��
	for (UINT i = 0; i < oSudokuMatrix.cols(); i++) {
		if (0 == oTmpMatrix(i)) {
			continue;
		}
		oTmpBlock.push_back(oTmpMatrix(i));
	}

	auto oItTmpBlock = oTmpBlock.begin();
	for (; oItTmpBlock != oTmpBlock.end(); ++oItTmpBlock) {
		oData.erase(std::remove(oData.begin(), oData.end(), *oItTmpBlock), oData.end());
	}
}
