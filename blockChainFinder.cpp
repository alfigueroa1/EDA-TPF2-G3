/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "blockChainFinder.h"
#include <iostream>
#include <fstream>
#include <intrin.h>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BLOCK_FIELDS 7
#define TRANS_FIELDS 5

/*******************************************************************************
 * CASS METHODS DEFINITIONS
 ******************************************************************************/
BlockChainFinder::BlockChainFinder() 
{
	error = NO_BLOCK_ERROR;
}

BlockChainFinder::~BlockChainFinder() 
{

}


vector<string>* BlockChainFinder::getValidJSONs(string path) 
{
	bool ret = true;
	if (getJSONNames(path)) 
	{											//If path contains .json files
		//auto i = jsonNames.begin();
		for (auto i = jsonNames.begin(); i != jsonNames.end(); i++) 
		{
			if (!isJsonAValidBlockChain(*i))
			{
				jsonNames.erase(i--);									//Removes invalid json files (json not a valid blockchain)
			}
		}
		if (jsonNames.empty())
		{
			ret = false;
		}
	}
	else
	{
		ret = false;
	}
	if (ret)
	{
		return &jsonNames;
	}
	else
	{
		return nullptr;
	}
}

bool BlockChainFinder::getJSONNames(string path) 
{
	bool ret = true;
	error = NO_BLOCK_ERROR;
	jsonNames.clear();

	string fileExtension;

	if (exists(path))	//Existe el path?
	{
		if (is_regular_file(path))	// El path es un archivo?
		{        
			cout << path << " size is " << file_size(path) << '\n';	//DEBUGGING
		}
		else if (is_directory(path))	//De no serlo, es una carpeta?
		{		
			//cout << "Is a directory" << endl;
			for (directory_entry& x : directory_iterator(path))	//Recorre toda la carpeta, elemento a elemento.
			{
				fileExtension = extension(x.path());			//Obtiene la extension del archivo
				if (!fileExtension.find(".json"))					//Y verifica si es .json.
				{
					string aux;
					aux = x.path().generic_path().string().c_str();
					jsonNames.push_back(aux);
					//cout << aux << " es un archivo .json valido" << std::endl; //DEBUGGING
				}
			}
		}
		else {
			cout << path << " exists, but is not a regular file or directory\n"; //DEBUGGING
			error = INVALID_PATH;
		}
	}
	else {
		cout << path << " does not exist\n";	//DEBUGGING
		error = INVALID_PATH;
		ret = false;
	}
	if (jsonNames.empty()) {
		error = NOT_FOUND;
		ret = false;
	}
	return ret;
}



//Elijan cual les gusta mas
void BlockChainFinder::saveBlockChain(BlockChain& blockchain, string path)
{
	std::ifstream i(path.c_str()); //Se puede cambiar, no se como recibo el JSON;
	json j;
	i >> j;

	for (auto& blocks : j)
	{
		//Block 
		Block block;

		auto height = blocks["height"];
		block.height = height;

		auto nonce = blocks["nonce"];
		block.nonce = nonce;

		auto blockId = blocks["blockid"];
		block.blockId = blockId.get<string>();

		auto prevBlockId = blocks["previousblockid"];
		block.previousBlockId = prevBlockId.get<string>();

		auto root = blocks["merkleroot"];
		block.merkleRoot = root.get<string>();

		auto nTx = blocks["nTx"];
		block.nTx = nTx;

		//Transactions
		auto arrayTrans = blocks["tx"];
		for (auto& trans : arrayTrans)
		{
			Transaction auxTrans;

			auto txId = trans["txid"];
			auxTrans.txId = txId.get<string>();

			auto nTxIn = trans["nTxin"];
			auxTrans.nTxIn = nTxIn;

			auto vIn = trans["vin"];
			for (auto& elsi : vIn)
			{
				Vin auxVin;

				auto tBlockId = elsi["blockid"];
				auxVin.blockId = tBlockId.get<string>();

				auto tTxId = elsi["txid"];
				auxVin.txId = tTxId.get<string>();

				auxTrans.vIn.push_back(auxVin);
			}

			auto nTxOut = trans["nTxout"];
			auxTrans.nTxOut = nTxOut;

			auto vOut = trans["vout"];
			for (auto& elso : vOut)
			{
				Vout auxVout;

				auto publicId = elso["publicid"];
				auxVout.publicId = publicId.get<string>();

				auto amount = elso["amount"];
				auxVout.amount = amount;

				auxTrans.vOut.push_back(auxVout);
			}

			block.tx.push_back(auxTrans);
		}

		blockchain.push_back(block);
	}
}

bool BlockChainFinder::isJsonAValidBlockChain(string path)
{
	bool ret = false;

	try
	{
		std::ifstream i(path.c_str());
		json j;

		i >> j;

		//Block
		for (auto& blocks : j)	//Parsea todos los bloques
		{
			if (blocks.size() == BLOCK_FIELDS) //Si son 7 elementos
			{
				blocks.at("height");
				blocks.at("nonce");
				blocks.at("blockid");	//Se fija que sean los correspondientes
				blocks.at("previousblockid");
				blocks.at("merkleroot");
				int ntx = blocks.at("nTx");
				blocks.at("height");
				blocks.at("tx");

				//Transactions
				auto arrayTrans = blocks["tx"];
				for (auto& trans : arrayTrans)	//Parsea todas las transacciones
				{
					if (arrayTrans.size() == ntx && trans.size() == TRANS_FIELDS)	//Si son 5 elementos
					{
						trans.at("txid");
						int txin = trans.at("nTxin");
						trans.at("vin");	//Se fija que sean los correctos
						int txout = trans.at("nTxout");
						trans.at("vout");

						auto vIn = trans["vin"];
						auto vOut = trans["vout"];
						if (vIn.size() == txin && vOut.size() == txout)
						{
							for (auto& elsi : vIn)
							{
								elsi.at("blockid");
								elsi.at("txid");
							}

							for (auto& elso : vOut)
							{
								elso.at("publicid");
								elso.at("amount");
							}

							ret = true;
						}
					}
				}
			}
		}
	}

	catch (std::exception& e)
	{
		ret = false;
	}

	return ret;
}


//bool BlockChainFinder::isJsonAValidBlockChain2()
//{
//	try
//	{
//		std::ifstream i("test.json");
//		json j;
//
//		i >> j;
//
//		//Block
//		for (auto& blocks : j)	//Parsea todos los bloques
//		{
//			if (blocks.size() != 7) //Si son 7 elementos
//			{
//				return false;
//			}
//
//			blocks.at("height");
//			blocks.at("nonce");
//			blocks.at("blockid");
//			blocks.at("previousblockid");
//			blocks.at("merkleroot");
//			blocks.at("nTx");
//			blocks.at("height");
//			blocks.at("tx");
//
//			//Transactions
//			auto arrayTrans = blocks["tx"];
//			for (auto& trans : arrayTrans)	//Parsea todas las transacciones
//			{
//				if (trans.size() != 5)	//Si son 5 elementos
//				{
//					return false;
//				}
//
//				trans.at("txid");
//				trans.at("nTxin");
//				trans.at("vin");	//Se fija que sean los correctos
//				trans.at("nTxout");
//				trans.at("vout");
//				auto vIn = trans["vin"];
//				auto vOut = trans["vout"];
//				for (auto& elsi : vIn)
//				{
//					if (elsi.size() != 2)
//					{
//						return false;
//					}
//
//					elsi.at("blockid");
//					elsi.at("txid");
//				}
//
//				for (auto& elso : vOut)
//				{
//					if (elso.size() != 2)
//					{
//						return false;
//					}
//
//					elso.at("publicid");
//					elso.at("amount");
//				}
//			}
//		}
//	}
//
//	catch (std::exception& e)
//	{
//		return false;
//	}
//
//	return true;
//}