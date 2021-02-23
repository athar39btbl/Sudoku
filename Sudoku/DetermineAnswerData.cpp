#include "DetermineAnswerData.h"

DataAnswer::DataAnswer() 
	: m_oDataSet(0)
{}

DataAnswer::~DataAnswer() {
	// 処理なし
}

// 答えを求める本処理
int DataAnswer::SolveSudokuMatrix(SUDOKU_MATRIX& oSudokuMatrix) {
	DATASET oDataSet                 = GetAnswerData();
	DATASET oDataSetBack             = oDataSet;
	SUDOKU_MATRIX oDataSudokuForSolve = oSudokuMatrix;

	auto oItDataSet = oDataSet.begin();
	// 上記DATASETの数だけループする
	while (oItDataSet != oDataSet.end()) {
		UINT uiRow = oItDataSet->uiRow;
		UINT uiColumn = oItDataSet->uiColumn; 

		// SUDOKU_MATRIXを解く前に、答えの候補がまだあるのか確認
		if ((oItDataSet->oAnswer).empty()) {
			// 削除した答えを元に戻す。
			if (RestoreAnswerList(*oItDataSet, oDataSetBack)) {
				// SUDOKU_MATRIXを解く前の状態に戻す
				oDataSudokuForSolve(uiRow, uiColumn) = 0;
			}

			// さかのぼって一番初め迄戻ってしまったら、解けないということなので処理終える
			if (oDataSet.begin() == oItDataSet) {
				return 1;
			}
			
			// イテレータを一個前に戻して処理を続ける
			--oItDataSet;
			continue;
		}

		// 答えの候補の数だけループを続ける
		auto oItAnswer = (oItDataSet->oAnswer).begin();
		while (oItAnswer != (oItDataSet->oAnswer).end()) {
			oDataSudokuForSolve(uiRow, uiColumn) = *oItAnswer;
			
			// 被りがあるのか確認
			bool bRet = DoExistOverlapNumber(oDataSudokuForSolve, uiRow, uiColumn);
			
			// 上で使った答えを削除して、答えの候補リスト用のイテレータを一個進める
			oItAnswer = (oItDataSet->oAnswer).erase(oItAnswer);

			if (!bRet) {
				// 被りがなければさっき使った答えを削除して、次のデータセットを試す
				++oItDataSet;
				break;
			}

			// oItDataSet 内の答えをもう試行できないなら・・・
			if ((oItDataSet->oAnswer).empty()) {
				// 削除した答えを元に戻す。
				if (RestoreAnswerList(*oItDataSet, oDataSetBack)) {
					// SUDOKU_MATRIXを解く前の状態に戻す
					oDataSudokuForSolve(uiRow, uiColumn) = 0;
				}

				// さかのぼって一番初め迄戻ってしまったら、解けないということなので処理終える
				if (oDataSet.begin() == oItDataSet) {
					return 1;
				}
				// イテレータを一個前に戻して処理を続ける
				--oItDataSet;
				break; 
			}
			// oItDataSet 内の答えがまだ残ってて処理できるなら・・・
			// oItDataSet 内の　次の答えに進む
			else {
				continue;
			}
		}
	}

	oSudokuMatrix = oDataSudokuForSolve;
	return 0;
}


// 空欄の場所データ情報と数独データ情報を、答え候補データクラスに渡す
void DataAnswer::CreateDataSet(const SUDOKU_MATRIX& oSudokuMatrix) {
	// 空欄の場所を調べて、結果を取得。
	DataEmpty oDataEmpty;
	oDataEmpty.DeterminePositionEmpty(oSudokuMatrix);
	
	DATASET oDataSet;

	// 空欄の場所を取得
	EMPTY_LIST oEmptyList = oDataEmpty.GetEmptyList();
	auto oItEmptyList = oEmptyList.begin();
	// 候補となる数と結び付けて、リストを作る
	for (; oItEmptyList != oEmptyList.end(); ++oItEmptyList) {
		// 行、列、箱の中の数全てを対象として、1,2,3,4,5,6,7,8,9の中で無い数を全て洗い出す。
		s_DataSetUnit oDataSetUnit;
		oDataSetUnit.uiRow    = oItEmptyList->first;
		oDataSetUnit.uiColumn = oItEmptyList->second;
		oDataSetUnit.oAnswer  = DetermineEnableToAddNumber(oSudokuMatrix, oDataSetUnit.uiRow, oDataSetUnit.uiColumn);

		oDataSet.push_back(oDataSetUnit);
	}

	// すぐ答えを出せるところから解くために、oAnswerのサイズが小さい順にソート
	std::sort(oDataSet.begin(), oDataSet.end(),
		[](const s_DataSetUnit& x, const s_DataSetUnit& y) 
			{ return x.oAnswer.size() < y.oAnswer.size(); }
		);

	// メンバ変数にセット
	SetAnswerData(oDataSet);
}

// メンバ変数のセッタ
const void DataAnswer::SetAnswerData(const DATASET& oDataSet) {
	m_oDataSet = oDataSet;
}
// メンバ変数のゲッタ
const DATASET DataAnswer::GetAnswerData() const {
	return m_oDataSet;
}

// 答えの候補リストの復元処理
bool DataAnswer::RestoreAnswerList(s_DataSetUnit& oDataSetUnit, const DATASET& oSudokuMatrix) {
	
	auto oItDataSetBack = oSudokuMatrix.begin();
	for (; oItDataSetBack != oSudokuMatrix.end(); ++oItDataSetBack) {
		if ((oItDataSetBack->uiRow == oDataSetUnit.uiRow) && (oItDataSetBack->uiColumn == oDataSetUnit.uiColumn)) {
			// 解く前の答えのリストに戻すために代入する
			oDataSetUnit.oAnswer = oItDataSetBack->oAnswer;
			return true;
		}
	}

	return false;
}

// 被りがあるところのチェック
// 存在してたらtrue
// 存在してなかったらfalse
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

// 被りがある行のチェック
bool DataAnswer::DoExistOverlapNumberRow(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow) {
	OVERLAP_LIST oTmpRow;
	oTmpRow.reserve(oSudokuMatrix.rows());
	// rowで同じ数がないか判定する
	for (UINT i = 0; i < oSudokuMatrix.rows(); i++) {
		if (0 == oSudokuMatrix(uiRow, i)) {
			continue;
		}
		oTmpRow.push_back(oSudokuMatrix(uiRow, i));
	}

	// 被りがあるか確認
	std::sort(oTmpRow.begin(), oTmpRow.end());
	auto oItRowUnique = std::unique(oTmpRow.begin(), oTmpRow.end());
	if (oItRowUnique == oTmpRow.end()) {
		return true;	// 存在してる
	}
	return false;
}

// 被りがある列のチェック
bool DataAnswer::DoExistOverlapNumberColumn(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiCol) {
	OVERLAP_LIST oTmpRow;
	oTmpRow.reserve(oSudokuMatrix.cols());

	// colで同じ数がないか判定する
	for (UINT i = 0; i < oSudokuMatrix.rows(); i++) {
		if (0 == oSudokuMatrix(i, uiCol)) {
			continue;
		}
		oTmpRow.push_back(oSudokuMatrix(i, uiCol));
	}

	// 被りがあるか確認
	std::sort(oTmpRow.begin(), oTmpRow.end());
	auto oItRowUnique = std::unique(oTmpRow.begin(), oTmpRow.end());
	if (oItRowUnique == oTmpRow.end()) {
		return true;	// 存在してる
	}
	return false;
}

// 被りがある箱のチェック
bool DataAnswer::DoExistOverlapNumberBlock(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiCol) {
	OVERLAP_LIST oTmpBlock;
	oTmpBlock.reserve(oSudokuMatrix.rows());

	UINT uiStartRow = uiRow / SIZE_BLOCK * SIZE_BLOCK;
	UINT uiStartCol = uiCol / SIZE_BLOCK * SIZE_BLOCK;
	Eigen::MatrixXd oTmpMatrix = oSudokuMatrix.block<SIZE_BLOCK, SIZE_BLOCK>(uiStartRow, uiStartCol);

	// blockで同じ数がないか判定する
	for (UINT i = 0; i < oSudokuMatrix.cols(); i++) {
		if (0 == oTmpMatrix(i)) {
			continue;
		}
		oTmpBlock.push_back(oTmpMatrix(i));
	}

	// 被りがあるか確認
	std::sort(oTmpBlock.begin(), oTmpBlock.end());
	auto oItRowUnique = std::unique(oTmpBlock.begin(), oTmpBlock.end());
	if (oItRowUnique == oTmpBlock.end()) {
		return true;	// 存在してる
	}
	return false;
}


// 答え候補データクラス内で、答えの候補を求める。
ANSWER_LIST DataAnswer::DetermineEnableToAddNumber(const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiColumn) {
	ANSWER_LIST oData = oWritableValues;

	DetermineEnableToAddNumberRow(oData, oSudokuMatrix, uiRow);

	DetermineEnableToAddNumberColumn(oData, oSudokuMatrix, uiColumn);

	DetermineEnableToAddNumberBlock(oData, oSudokuMatrix, uiRow, uiColumn);

	return oData;
}

// 答え候補データクラス内で、行の中で答えの候補を求める。
void DataAnswer::DetermineEnableToAddNumberRow(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow) {
	ANSWER_LIST oTmpRow;
	// rowで同じ数がないか判定する
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

// 答え候補データクラス内で、列の中で答えの候補を求める。
void DataAnswer::DetermineEnableToAddNumberColumn(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiColumn) {
	ANSWER_LIST oTmpCol;
	// columnで同じ数がないか判定する
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

// 答え候補データクラス内で、ブロックの中で答えの候補を求める。
void DataAnswer::DetermineEnableToAddNumberBlock(ANSWER_LIST& oData, const SUDOKU_MATRIX& oSudokuMatrix, const UINT& uiRow, const UINT& uiColumn) {
	ANSWER_LIST oTmpBlock;

	// ブロックの左上座標を決める
	UINT uiOrgRow = uiRow / SIZE_BLOCK * SIZE_BLOCK;
	UINT uiOrgCol = uiColumn / SIZE_BLOCK * SIZE_BLOCK;
	Eigen::MatrixXd oTmpMatrix = oSudokuMatrix.block<SIZE_BLOCK, SIZE_BLOCK>(uiOrgRow, uiOrgCol);

	// blockで同じ数がないか判定する
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
