#include "server.h"
Server::Server(){this->clients={};}
std::vector<std::string> pending_trxs = {};
std::shared_ptr<Client> Server::add_client(std::string id) {
    // 检查客户端的id是否已经存在
    for (auto& client : clients) {
        if (client.first->get_id() == id) {
            std::string random_digits = std::to_string(rand() % 9000 + 1000);
            id += random_digits;
            break;
        }
    }
    std::shared_ptr<Client> client = std::make_shared<Client>(id,*this);
    clients[client] = 5.0; // 初始钱包值为5个硬币

    return client;
}
std::shared_ptr<Client> Server::get_client(std::string id) const {
    for (auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.first;
        }
    }
    return nullptr;
}
double Server::get_wallet(std::string id) const{
    for (auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.second;
        }
    }
    return 0.0;
}
std::map<std::shared_ptr<Client>,double> Server::get_clients() const {
    return clients;
}
void show_wallets(const Server& server){
    std::map<std::shared_ptr<Client>,double> clients=server.get_clients();
    for (auto& client : clients) {
        std::cout << "Client " << client.first->get_id() << " has " << client.second << " coins." << std::endl;
    }
}
bool Server::parse_trx(const std::string& trx, std::string& sender, std::string& receiver, double& value) {
    // 使用 std::string_view 提高性能
    std::string_view trx_view = trx;

    // 找到第一个和最后一个 '-' 的位置
    auto first_dash = trx_view.find('-');
    auto last_dash = trx_view.rfind('-');

    // 检查格式是否合法
    if (first_dash == std::string_view::npos || last_dash == std::string_view::npos || first_dash == last_dash) {
        throw std::runtime_error("Invalid transaction format: missing or misplaced '-'");
    }

    // 提取 sender, receiver 和 value
    sender = std::string(trx_view.substr(0, first_dash)); // 截取发送者
    receiver = std::string(trx_view.substr(first_dash + 1, last_dash - first_dash - 1)); // 截取接收者

    // 尝试解析 value
    try {
        value = std::stod(std::string(trx_view.substr(last_dash + 1)));
    } catch (const std::exception&) {
        throw std::runtime_error("Invalid transaction format: unable to parse value");
    }

    // 成功解析返回 true
    return true;
}
bool Server::add_pending_trx(std::string& trx, std::string signature) const {
    std::string sender, receiver;
    double value;
    if(!parse_trx(trx,sender,receiver,value)){
        return false;
    }
    std::shared_ptr<Client> sender_client = get_client(sender);
    std::shared_ptr<Client> receiver_client = get_client(receiver);
    //检查发送者是否存在
    if(sender_client == nullptr){
        return false;
    }
    //检查接收者是否存在
    if(receiver_client == nullptr){
        return false;
    }
    //验证签名
    if(!crypto::verifySignature(sender_client->get_publickey(), trx, signature)){
        return false;
    }
    //检查余额
    if(sender_client->get_wallet() < value){
        return false;
    }
    //添加交易
    pending_trxs.push_back(trx);
    return true;
}
size_t Server::mine() {
    // 检查是否有待处理的交易
    if (pending_trxs.empty()) {
        return 0;
    }
    //处理交易
    for (auto& trx : pending_trxs) {
        std::string sender, receiver;
        double value;
        parse_trx(trx, sender, receiver, value);
        std::shared_ptr<Client> sender_client = get_client(sender);
        std::shared_ptr<Client> receiver_client = get_client(receiver);
        clients[sender_client] -= value;
        clients[receiver_client] += value;
    }

    // 挖矿逻辑
    std::string mempool{};
    for (auto& trx : pending_trxs) {
        mempool += trx;
    }

    while (true) { // 无限循环，直到挖矿成功
        for (auto& client : clients) {
            size_t nonce = client.first->generate_nonce(); // 生成随机 nonce
            std::string temp_mempool = mempool + std::to_string(nonce);
            std::string hash = crypto::sha256(temp_mempool);
            
            if (hash.substr(0, 10).find("000") != std::string::npos) {
                // 挖矿成功
                std::cout << "Miner: " << client.first->get_id() << std::endl;
                client.second += 6.25; // 矿工奖励
                return nonce;
            }
        }
    }
}

