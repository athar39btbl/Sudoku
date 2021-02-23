#include <iostream>
#include "BaseSudokuData.h"

#define MAX_LENGTH_STRING (256)
#define INPUTFILENAME (".\\Question.csv")

int main(int argc, char* argv[]) {
	// csv�t�@�C���I�[�v��
	char strFileName[MAX_LENGTH_STRING] = "";
	if (argc <= 1) {
		strncpy_s(strFileName, INPUTFILENAME, MAX_LENGTH_STRING);
	}
	else if (2 == argc) {
		strncpy_s(strFileName, argv[1], MAX_LENGTH_STRING);
	}
	else {
		return 1;
	}

	// csv�t�@�C���̓ǂݍ���
	// ��L�𐔓ƃf�[�^�C���v�b�g�Ƃ��ăZ�b�g
	DataSudoku oSudokuData;
	if (!oSudokuData.LoadSudokuFile(strFileName)) {
		return 1;
	}

	// �C���v�b�g�̌��ʂ��o��
	SUDOKU_MATRIX oMatrix = Eigen::MatrixXd::Zero(9, 9);
	oMatrix = oSudokuData.GetSudokuData();
	std::cout << "���I�I" << std::endl;
	std::cout << oMatrix << std::endl;

	// ��L�C���v�b�g���p�����[�^�Ƃ��ēn���Đ��Ƃ������B�A�E�g�v�b�g�͉����I�������̃C���v�b�g
	if (oSudokuData.ExecToSolveSudoku()) {
		return 1;
	}
	
	// �C���v�b�g�̌��ʂ��o��
	oMatrix = oSudokuData.GetSudokuData();

	std::cout << "\n\n����" << std::endl;
	std::cout << oMatrix << std::endl;

	return 0;
}