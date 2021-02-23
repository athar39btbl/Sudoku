#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <Eigen/Dense>

#ifndef UTILITY_H
#define UTILITY_H

typedef unsigned int UINT;

// oIfs���J���}��؂肵�A���ʂ�oData�Ɋi�[
static void Tokenize(std::vector<UINT>& oData, std::string& strBuffer, const char cDelimiter = ',') {
	//strBuffer����o�͉\�ɕϊ�����
	std::stringstream oStringStream{ strBuffer };
	std::string strTmp;

	// �J���}��؂育�ƂɃf�[�^���o
	int iCnt = 0;
	while (std::getline(oStringStream, strTmp, cDelimiter)) {
		//while (std::getline(oStringStream, strTmp, cDelimiter.c_str())) {	// �����ł����ƑS���̃J���}��؂��ǂݍ���ł���Ȃ��B
		int num = atoi(strTmp.c_str());
		oData.push_back(num);
	}
}

// �^�ϊ�(std::vector<std::vector<UINT>> -> Eigen::MatrixXd)
static bool ConvertVectorToMatix(Eigen::MatrixXd oMatrix, const std::vector<std::vector<UINT>> oVector) {
	bool bRet = false;

	if (oMatrix.rows() == oVector[0].size()) {
		return bRet;
	}
	if (oMatrix.cols() == oVector.capacity()) {
		return bRet;
	}

	// �ϊ�����(std::vector<std::vector<UINT>> -> Eigen::MatrixXd)
	for (int i = 0; i < oMatrix.rows() - 1; i++) {
		for (int j = 0; j < oMatrix.cols() - 1; j++) {
			oMatrix(i, j) = oVector[i][j];
		}
	}
}


#endif // UTILITY_H