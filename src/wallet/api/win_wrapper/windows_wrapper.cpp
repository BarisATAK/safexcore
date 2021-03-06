//
// Created by stefan on 28.11.18..
//

#include <vector>
#include <string>
#include <iostream>

#include <cstdlib>
#include <cstring>
#include <stdio.h>

#include "windows_wrapper.h"
#include "../wallet.h"
#include "../pending_transaction.h"
#include "../transaction_info.h"
#include "../wallet_manager.h"
#include "../wallet_api.h"
#include "windows_wallet_listener.h"
#include "../transaction_history.h"


char *returnStdString(std::string &&in)
{
  char *dst = (char *) malloc(in.size() * sizeof(char));
  memcpy(dst, in.c_str(), in.size());
  return dst;
}

extern "C" DLL_MAGIC  void win_checkDLL(const char *msg)
{
  printf("Message from below: %s \n", msg);
  std::cout << "message bab ba std::cout " << std::endl;
  fflush(stdout);

}

extern "C" DLL_MAGIC  void *win_createWallet(uint8_t nettype)
{

  printf("Called %s \n", __FUNCTION__);
  Safex::WalletImpl *wallet = new Safex::WalletImpl(static_cast<Safex::NetworkType>(nettype));
  return static_cast<void *>(wallet);
}

extern "C" DLL_MAGIC  void win_deleteWallet(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  if (wallet)
  {
    delete wallet;
  }
}

extern "C" DLL_MAGIC  uint8_t win_initB(void *self, const char *daemon_address)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return static_cast<uint8_t>(wallet->init(daemon_address));
}

extern "C" DLL_MAGIC  void win_startRefresh(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  wallet->startRefresh();
}

extern "C" DLL_MAGIC  uint8_t win_storeB(void *self, const char *path)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return static_cast<bool>(wallet->store(path));
}

extern "C" DLL_MAGIC  void *win_createTransaction(
        void *self,
        const char *dst_addr,
        const char *payment_id,
        uint64_t value_amount,
        uint32_t mixin_count,
        uint32_t priority,
        uint32_t subaddr_account,
        uint32_t subaddr_indices,
        uint32_t tx_type)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  Safex::PendingTransaction *pTx = wallet->createTransaction(
          dst_addr,
          payment_id,
          value_amount,
          mixin_count,
          static_cast<Safex::PendingTransaction::Priority>(priority),
          0,
          {},
          static_cast<Safex::TransactionType>(tx_type)
  );

  return static_cast<void *>(pTx);
}

extern "C" DLL_MAGIC const char *win_address(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  printf("Called %s \n", __FUNCTION__);
  static char buffer[512];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, wallet->address().c_str(), wallet->address().length());
  buffer[wallet->address().length()+1] = '\0';
  return (const char *) buffer;

}
extern "C" DLL_MAGIC const char *win_seed(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);

  static char buffer[1024];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, wallet->seed().c_str(), wallet->seed().length());
  buffer[wallet->seed().length() +1] = '\0';
  return (const char *) buffer;
}
extern "C" DLL_MAGIC const char *win_path(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  static char buffer[1024];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, wallet->path().c_str(), wallet->path().length());
  buffer[wallet->path().length()+1] = '\0';
  return (const char *) buffer;
}
extern "C" DLL_MAGIC  uint8_t win_nettype(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);

  return static_cast<uint8_t>(wallet->nettype());
}
extern "C" DLL_MAGIC const char *win_secretViewKey(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  static char buffer[128];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, wallet->secretViewKey().c_str(), wallet->secretViewKey().length());
  buffer[wallet->secretViewKey().length() + 1] = '\0';
  return (const char *) buffer;
}
extern "C" DLL_MAGIC const char *win_publicViewKey(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  static char buffer[128];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, wallet->publicViewKey().c_str(), wallet->publicViewKey().length());
  buffer[wallet->publicViewKey().length() + 1] = '\0';
  return (const char *) buffer;
}
extern "C" DLL_MAGIC const char *win_secretSpendKey(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  static char buffer[128];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, wallet->secretSpendKey().c_str(), wallet->secretSpendKey().length());
  buffer[wallet->secretSpendKey().length() + 1] = '\0';
  return (const char *) buffer;
}
extern "C" DLL_MAGIC const char *win_publicSpendKey(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  static char buffer[128];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, wallet->publicSpendKey().c_str(), wallet->publicSpendKey().length());
  buffer[wallet->publicSpendKey().length() + 1] = '\0';
  return (const char *) buffer;
}
extern "C" DLL_MAGIC  uint8_t win_setPasswordB(void *self, const char *pass_c)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  std::string password(pass_c);

  return static_cast<uint8_t>(wallet->setPassword(password));
}
extern "C" DLL_MAGIC const char *win_errorString(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  static char buffer[2048];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, wallet->errorString().c_str(), wallet->errorString().length());
  return (const char *) buffer;
}
extern "C" DLL_MAGIC  void win_setRefreshFromBlockHeight(void *self, uint32_t height)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  wallet->setRefreshFromBlockHeight(height);
}

extern "C" DLL_MAGIC uint64_t win_getRefreshFromBlockHeight(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return wallet->getRefreshFromBlockHeight();
}

extern "C" DLL_MAGIC  uint32_t win_connected(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);

  return static_cast<uint32_t>(wallet->connected());
}

extern "C" DLL_MAGIC uint8_t win_refresh(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return static_cast<uint8_t>(wallet->refresh());
}

extern "C" DLL_MAGIC  void win_setTrustedDaemon(void *self, uint8_t argB)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  bool arg = (argB == 0);
  wallet->setTrustedDaemon(arg);
}
extern "C" DLL_MAGIC  uint8_t win_trustedDaemonB(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);

  return static_cast<uint8_t>(wallet->trustedDaemon());
}
extern "C" DLL_MAGIC  uint64_t win_balanceAll(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);

  return wallet->balanceAll();
}
extern "C" DLL_MAGIC  uint64_t win_unlockedBalanceAll(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);

  return wallet->unlockedBalanceAll();
}
extern "C" DLL_MAGIC  uint64_t win_tokenBalanceAll(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);

  return wallet->tokenBalanceAll();
}
extern "C" DLL_MAGIC  uint64_t win_unlockedTokenBalanceAll(void *self)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);

  return wallet->unlockedTokenBalanceAll();
}

extern "C" DLL_MAGIC uint8_t win_synchronizedB(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return static_cast<uint8_t>(wallet->synchronized());
}

extern "C" DLL_MAGIC void win_setAutoRefreshInterval(void* self, uint32_t millis) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  wallet->setAutoRefreshInterval(millis);
}

extern "C" DLL_MAGIC  uint8_t win_static_addressValid(const char *address, uint32_t nettype)
{
  return static_cast<uint8_t>(Safex::Wallet::addressValid(address, static_cast<Safex::NetworkType>(nettype)));
}

extern "C" DLL_MAGIC const char *win_GenPaymentId()
{
  return Safex::Wallet::genPaymentId().c_str();
}

extern "C" DLL_MAGIC  uint8_t win_PaymentIdValid(const char *pid)
{
  return static_cast<uint8_t>(Safex::Wallet::paymentIdValid(pid));
}

extern "C" DLL_MAGIC  void win_SetListener(void *self, void *listener)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(listener);
  wallet->setListener(wlstn);
}

extern "C" DLL_MAGIC  void win_segregatePreForkOutputs(void *self, uint8_t segregateB)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  wallet->segregatePreForkOutputs(static_cast<bool>(segregateB));
}

extern "C" DLL_MAGIC  void win_keyReuseMitigation2(void *self, uint8_t mitigationB)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  wallet->keyReuseMitigation2(static_cast<bool>(mitigationB));
}

extern "C" DLL_MAGIC const char *win_IntegratedAddress(void *self, const char *paymentId)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return wallet->integratedAddress(paymentId).c_str();
}

extern "C" DLL_MAGIC uint64_t win_blockChainHeight(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return wallet->blockChainHeight();
}

extern "C" DLL_MAGIC uint64_t win_approximateBlockChainHeight(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return wallet->approximateBlockChainHeight();
}

extern "C" DLL_MAGIC uint64_t win_daemonBlockChainHeight(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return wallet->daemonBlockChainHeight();
}

extern "C" DLL_MAGIC uint64_t win_daemonBlockChainTargetHeight(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return wallet->daemonBlockChainTargetHeight();
}

extern "C" DLL_MAGIC bool win_rescanBlockchain(void* self) {
	Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
	return static_cast<uint8_t>(wallet->rescanBlockchain());
}

extern "C" DLL_MAGIC void win_rescanBlockchainAsync(void* self) {
	Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
	wallet->rescanBlockchainAsync();
}

extern "C" DLL_MAGIC void win_setSeedLanguage(void* self, const char* seedLanguage) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  std::string seed(seedLanguage);
  wallet->setSeedLanguage(seed);
}


extern "C" DLL_MAGIC void* win_history(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return static_cast<void*>(wallet->history());
}


extern "C" DLL_MAGIC const char* win_addrbook_get_all(void* self) {
  static std::string result;
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  std::vector<Safex::AddressBookRow*> rows = wallet->addressBook()->getAll();

  result.clear();
  result = "";
  std::string delimeter{"$@"};
  for(auto row : rows) {
    std::string serialized_row = std::to_string(row->getRowId()) + delimeter;
    serialized_row += row->getAddress() + delimeter;
    serialized_row += row->getPaymentId() + delimeter;
    serialized_row += row->getDescription() + delimeter;

    result += serialized_row;
  }

  return result.c_str();
}

extern "C" DLL_MAGIC uint8_t win_addrbook_add_row(void* self, const char* addr, const char* pid, const char* desc) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return static_cast<uint8_t>(wallet->addressBook()->addRow(addr, pid, desc));
}

extern "C" DLL_MAGIC uint8_t win_addrbook_del_row(void* self, uint32_t row_id) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return static_cast<uint8_t>(wallet->addressBook()->deleteRow(row_id));
}

extern "C" DLL_MAGIC const char* win_addrbook_err_str(void* self) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return wallet->addressBook()->errorString().c_str();
}

extern "C" DLL_MAGIC int32_t win_addrbook_look_for_pid(void* self, const char* pid) {
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(self);
  return wallet->addressBook()->lookupPaymentID(pid);
}

/****************************** PENDING TRANSACTION API ***************************************************************/
extern "C" DLL_MAGIC  void *win_pt_create(void *in)
{
  Safex::WalletImpl *wallet = static_cast<Safex::WalletImpl *>(in);
  Safex::PendingTransactionImpl *ret = new Safex::PendingTransactionImpl(*wallet);
  return static_cast<void *>(ret);
}

extern "C" DLL_MAGIC  void win_pt_delete(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);
  if (ptx)
  {
    delete ptx;
  }
}

extern "C" DLL_MAGIC  uint64_t win_pt_amount(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  return ptx->amount();
}

extern "C" DLL_MAGIC  uint64_t win_pt_tokenAmount(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  return ptx->tokenAmount();
}

extern "C" DLL_MAGIC  uint64_t win_pt_dust(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  return ptx->dust();
}

extern "C" DLL_MAGIC  uint64_t win_pt_fee(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  return ptx->fee();
}

extern "C" DLL_MAGIC  uint64_t win_pt_txCount(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  return ptx->txCount();
}

extern "C" DLL_MAGIC  char *win_pt_txid(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  std::vector<std::string> ret = ptx->txid();
  static unsigned char buffer[256*1024];
  memset((void*)buffer, 0, sizeof(buffer));

  uint64_t offset = 0;
  for (auto &tx : ret)
  {
    memcpy(buffer + offset, tx.c_str(), 64);
    offset += 64;
  }

  return (char *)buffer;
}

extern "C" DLL_MAGIC  int32_t win_pt_status(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  return ptx->status();
}

extern "C" DLL_MAGIC const char *win_pt_errorString(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  return ptx->errorString().c_str();
}

extern "C" DLL_MAGIC  uint8_t win_pt_commit(void *self)
{
  Safex::PendingTransaction *ptx = static_cast<Safex::PendingTransaction *>(self);

  return static_cast<uint8_t>(ptx->commit());
}

/****************************** END PENDING TRANSACTION API ***********************************************************/


/****************************** WALLET MANAGER API ********************************************************************/
extern "C" DLL_MAGIC  void win_mng_closeWallet(void *self, void *wallet, uint8_t storeB)
{
  Safex::WalletManagerImpl *mngr = static_cast<Safex::WalletManagerImpl *>(self);
  Safex::WalletImpl *wllt = static_cast<Safex::WalletImpl *>(wallet);
  mngr->closeWallet(wllt, static_cast<bool>(storeB));
}

// @return Safex::WalletImpl
extern "C" DLL_MAGIC  void *win_mng_createWallet(void *self, const char *path, const char *password, const char *lang, uint32_t nettype)
{
  Safex::WalletManagerImpl *mngr = static_cast<Safex::WalletManagerImpl *>(self);
  return static_cast<void *>(mngr->createWallet(path, password, lang, static_cast<Safex::NetworkType>(nettype)));
}

// @return Safex::WalletImpl
extern "C" DLL_MAGIC  void *win_mng_openWallet(void *self, const char *path, const char *password, uint32_t nettype)
{
  Safex::WalletManagerImpl *mngr = static_cast<Safex::WalletManagerImpl *>(self);
  return static_cast<void *>(mngr->openWallet(path, password, static_cast<Safex::NetworkType>(nettype)));
}

// @return Safex::WalletImpl
extern "C" DLL_MAGIC  void *win_mng_recoveryWallet(
        void *self,
        const char *path,
        const char *password,
        const char *mnemonic,
        uint32_t nettype,
        uint64_t restoreHeight)
{
  Safex::WalletManagerImpl *mngr = static_cast<Safex::WalletManagerImpl *>(self);
  return static_cast<void *>(mngr->recoveryWallet(path, password, mnemonic, static_cast<Safex::NetworkType>(nettype), restoreHeight));
}

extern "C" DLL_MAGIC  uint8_t win_mng_walletExists(void *self, const char *path)
{
  Safex::WalletManagerImpl *mngr = static_cast<Safex::WalletManagerImpl *>(self);
  return static_cast<uint8_t>(mngr->walletExists(path));
}

extern "C" DLL_MAGIC  void *win_mngf_getWalletManager()
{
  Safex::WalletManager *mngr = Safex::WalletManagerFactory::getWalletManager();
  return static_cast<void *>(mngr);
}

extern "C" DLL_MAGIC void* win_mng_createWalletFromKeys(void* self, const char* path, const char* password, const char* language, uint32_t nettype,
                                                uint64_t restoreHeight,const char *addressString, const char* viewKeyString, const char* spendKeyString) {
  Safex::WalletManagerImpl *mngr = static_cast<Safex::WalletManagerImpl *>(self);
  return static_cast<void*>(mngr->createWalletFromKeys(path, password, language, static_cast<Safex::NetworkType>(nettype), restoreHeight, addressString, viewKeyString, spendKeyString));
}
/****************************** END WALLET MANAGER API ****************************************************************/
/****************************** TRANSACTIONINFO API *******************************************************************/
extern "C" DLL_MAGIC  void *win_txinfo_createTransactionInfo()
{
  Safex::TransactionInfoImpl *txInfo = new Safex::TransactionInfoImpl();
  return static_cast<void *>(txInfo);
}

extern "C" DLL_MAGIC  void win_txinfo_deleteTransactionInfo(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  if (txInfo)
  {
    delete txInfo;
  }
}

extern "C" DLL_MAGIC  int32_t win_txinfo_direction(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return static_cast<int32_t>(txInfo->direction());
}

extern "C" DLL_MAGIC  uint8_t win_txinfo_isPendingB(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return static_cast<uint8_t>(txInfo->isPending());
}

extern "C" DLL_MAGIC  uint8_t win_txinfo_isFailedB(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return static_cast<uint8_t>(txInfo->isFailed());
}

extern "C" DLL_MAGIC  uint64_t win_txinfo_amount(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return txInfo->amount();
}

extern "C" DLL_MAGIC  uint64_t win_txinfo_token_amount(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return txInfo->token_amount();
}

extern "C" DLL_MAGIC  uint64_t win_txinfo_fee(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return txInfo->fee();
}

extern "C" DLL_MAGIC  uint64_t win_txinfo_blockHeight(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return txInfo->blockHeight();
}

extern "C" DLL_MAGIC const char *win_txinfo_label(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  static unsigned char buffer[512];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, txInfo->label().c_str(),txInfo->label().length());
  buffer[txInfo->label().length()+1] = '\0';
  return (const char *) buffer;
}

extern "C" DLL_MAGIC const char *win_txinfo_hash(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  static unsigned char buffer[128];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, txInfo->hash().c_str(),txInfo->hash().length());
  buffer[64] = '\0';
  return (const char *) buffer;
}

extern "C" DLL_MAGIC  uint64_t win_txinfo_timestamp(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return static_cast<uint64_t>(txInfo->timestamp());
}

extern "C" DLL_MAGIC const char *win_txinfo_paymentId(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  static unsigned char buffer[256];
  memset((void *) buffer, 0, sizeof(buffer));
  memcpy((void *) buffer, txInfo->paymentId().c_str(),txInfo->paymentId().length());
  buffer[txInfo->paymentId().length()+1] = '\0';
  return (const char *) buffer;
}

extern "C" DLL_MAGIC  char *win_txinfo_transfers(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  const std::vector<Safex::TransactionInfo::Transfer> &transfers = txInfo->transfers();

  static char buffer[2048*1024+sizeof(uint32_t)];
  uint32_t offset = 0;
  memset(buffer, 0, sizeof(buffer));
  uint32_t size = static_cast<uint32_t>(transfers.size());
  memcpy(buffer+offset, &size, sizeof(uint32_t));
  offset += sizeof(uint32_t);

  for(auto& tx : transfers) {
    if(2048 * 1024 - offset <= 128) break;
    memcpy(buffer + offset, &(tx.amount), sizeof(uint64_t));
    offset += sizeof(uint64_t);
    memcpy(buffer + offset, &(tx.token_amount), sizeof(uint64_t));
    offset += sizeof(uint64_t);
    offset++;
  }
  buffer[2048*1024+sizeof(uint32_t)] = '\0';
  return static_cast<char *>(buffer);
}

extern "C" DLL_MAGIC  uint64_t win_txinfo_confirmations(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return txInfo->confirmations();
}

extern "C" DLL_MAGIC  uint64_t win_txinfo_unlockTime(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return static_cast<uint64_t>(txInfo->unlockTime());
}

extern "C" DLL_MAGIC  uint32_t win_txinfo_transactionType(void *self)
{
  Safex::TransactionInfoImpl *txInfo = static_cast<Safex::TransactionInfoImpl *>(self);
  return static_cast<uint32_t>(txInfo->transactionType());
}

/****************************** END TRANSACTIONINFO API ***************************************************************/

/****************************** WALLET LISTENER API ********************************************************************/
extern "C" DLL_MAGIC  void *win_lstn_Create(void *up)
{
  return static_cast<void *>(new WinWalletListener(up));
}
extern "C" DLL_MAGIC void win_lstn_setMoneySpent(void *self, void(*moneySpent_)(void *, const char *, uint64_t))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->moneySpent_ = moneySpent_;
}
extern "C" DLL_MAGIC void win_lstn_setMoneyReceived(void *self, void(*moneyReceived_)(void *, const char *, uint64_t))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->moneyReceived_ = moneyReceived_;
}

extern "C" DLL_MAGIC void win_lstn_setUnconfirmedMoneyReceived(void *self, void(*unconfirmedMoneyReceived_)(void *, const char *, uint64_t))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->unconfirmedMoneyReceived_ = unconfirmedMoneyReceived_;
}
extern "C" DLL_MAGIC void win_lstn_setTokensSpent(void *self, void(*tokensSpent_)(void *, const char *, uint64_t))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->tokensSpent_ = tokensSpent_;
}

extern "C" DLL_MAGIC void win_lstn_setTokenReceived(void *self, void(*tokenReceived_)(void *, const char *, uint64_t))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->tokenReceived_ = tokenReceived_;
}

extern "C" DLL_MAGIC void win_lstn_setUnconfirmedTokenReceived(void *self, void(*unconfirmedTokenReceived_)(void *, const char *, uint64_t))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->unconfirmedTokenReceived_ = unconfirmedTokenReceived_;
}

extern "C" DLL_MAGIC void win_lstn_setNewBlock(void *self, void(*newBlock_)(void *, uint64_t))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->newBlock_ = newBlock_;
}

extern "C" DLL_MAGIC void win_lstn_setUpdated(void *self, void(*updated_)(void *))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->updated_ = updated_;
}
extern "C" DLL_MAGIC void win_lstn_setRefreshed(void *self, void(*refreshed_)(void *))
{
  WinWalletListener *wlstn = static_cast<WinWalletListener *>(self);
  wlstn->refreshed_ = refreshed_;
}

/****************************** END WALLET LISTNER API ****************************************************************/

/****************************** OTHER FUNCTIONS ***********************************************************************/
extern "C" DLL_MAGIC void win_mlog_set_log_levelI(int level) {
  mlog_set_log_level(level);
}

extern "C" DLL_MAGIC void win_mlog_set_log_levelCPtr(const char* log) {
  mlog_set_log(log);
}
/****************************** END OTHER FUNCTIONS *******************************************************************/

/****************************** TRANSACTION HISTORY API ***************************************************************/
extern "C" DLL_MAGIC void* _hry_Create(void* wallet) {
  Safex::WalletImpl* wlt = static_cast<Safex::WalletImpl*>(wallet);
  Safex::TransactionHistoryImpl* ret = new Safex::TransactionHistoryImpl(wlt);
  return static_cast<void*>(ret);
}

extern "C" DLL_MAGIC void win_txhist_Delete(void* self) {
  if (self == nullptr) { return ;}
  Safex::TransactionHistoryImpl* txHist = static_cast<Safex::TransactionHistoryImpl*>(self);
  delete txHist;
}

extern "C" DLL_MAGIC uint32_t win_txhist_count(void* self) {
  Safex::TransactionHistoryImpl* txHist = static_cast<Safex::TransactionHistoryImpl*>(self);
  return txHist->count();
}

extern "C" DLL_MAGIC void* win_txhist_transactionInt(void* self, uint32_t index) {
  Safex::TransactionHistoryImpl* txHist = static_cast<Safex::TransactionHistoryImpl*>(self);
  Safex::TransactionInfo* txInfo = txHist->transaction(index);
  return static_cast<void*>(txInfo);
}

extern "C" DLL_MAGIC void* win_txhist_transactionStr(void* self, const char* id) {
  Safex::TransactionHistoryImpl* txHist = static_cast<Safex::TransactionHistoryImpl*>(self);
  Safex::TransactionInfo* txInfo = txHist->transaction(id);
  return static_cast<void*>(txInfo);
}

extern "C" DLL_MAGIC void** win_txhist_getAll(void* self, uint32_t* size) {
  Safex::TransactionHistoryImpl* txHist = static_cast<Safex::TransactionHistoryImpl*>(self);
  std::vector<Safex::TransactionInfo*> txInfos = txHist->getAll();
  *size = static_cast<uint32_t>(txInfos.size());
  static void* buffer[4096];
  std::sort(txInfos.begin(), txInfos.end(), [](Safex::TransactionInfo* a, Safex::TransactionInfo* b){
    return a->timestamp() > b->timestamp();
  });
  memset(buffer, 0, sizeof(buffer));
  size_t i = 0;
  for(auto tx : txInfos) {
    if(i >= 4096) break;
    buffer[i] = static_cast<void*>(tx);
    ++i;
  }

  return buffer;
}

extern "C" DLL_MAGIC void win_txhist_refresh(void* self) {
  Safex::TransactionHistoryImpl* txHist = static_cast<Safex::TransactionHistoryImpl*>(self);
  txHist->refresh();
}

/****************************** END TRANSACTION HISTORY API ***********************************************************/
