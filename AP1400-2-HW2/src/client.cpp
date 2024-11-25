#include "client.h"
Client::Client(std::string id, const Server& server) : id(id), server(&server) {
    std::string public_key{}, private_key{};
    crypto::generate_key(public_key, private_key);
    this->public_key = public_key;
    this->private_key = private_key;
}
std::string Client::get_id(){
    return this->id;
}
double Client::get_wallet(){
    return server->get_wallet(this->id);
}
std::string Client::get_publickey() const {
    return this->public_key;
}
std::string Client::sign(std::string txt) const {
    return crypto::signMessage(this->private_key,txt);
}
bool Client::transfer_money(std::string receiver, double value){
    //创建交易字符串
    std::string transaction = this->id + "-" + receiver + "-" + std::to_string(value);
    //签名交易
    std::string signature = this->sign(transaction);
    //发送交易
    return server->add_pending_trx(transaction, signature);
}
size_t Client::generate_nonce() {
    // 创建一个随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    // 创建一个均匀分布的随机数生成器
    std::uniform_int_distribution<> dis(0, 1000);
    // 生成一个随机数
    return dis(gen);
}