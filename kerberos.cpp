#include<bits/stdc++.h>
using namespace std;

class SimpleCrypto{
public:
    static string encrypt(string key, string data){
        string res = data;
        for(int i = 0; i<(int)res.size(); i++)
            res[i] = res[i] ^ key[i % key.size()];
        return "[ENC]:" + res;
    }
    static string decrypt(string key, string enc){
        if(enc.size() < 6 || enc.substr(0,6) != "[ENC]:") return enc;
        string data = enc.substr(6);
        string res = data;
        for(int i = 0; i<(int)res.size(); i++)
            res[i] = res[i] ^ key[i % key.size()];
        return res;
    }
};

struct Ticket{
    string sessionKey, clientId, clientAddress, serverId;
    time_t timestamp;
    int lifetime;
    string toString() const{
        stringstream ss;
        ss<<"Ticket[Client:"<<clientId<<",Server:"<<serverId
          <<",Key:"<<sessionKey<<",Time:"<<timestamp<<"]";
        return ss.str();
    }
};

struct Authenticator{
    string clientId, clientAddress;
    time_t timestamp;
    string toString() const{
        stringstream ss;
        ss<<"Auth[Client:"<<clientId<<",Time:"<<timestamp<<"]";
        return ss.str();
    }
};

class AuthenticationServer{
    unordered_map<string,string> users;
    string tgsKey;
public:
    AuthenticationServer(){
        users["alice"] = "password123";
        users["bob"]   = "securepass";
        users["admin"] = "admin123";
        tgsKey = "TGS_MASTER_KEY_2024";
    }
    string authenticateClient(string clientId, string tgsId, time_t ts){
        if(users.find(clientId) == users.end()) return "ERROR";
        string sk = "SK_AS_" + clientId + "_" + to_string(ts);
        Ticket t;
        t.sessionKey    = sk;
        t.clientId      = clientId;
        t.clientAddress = "192.168.1.100";
        t.serverId      = tgsId;
        t.timestamp     = ts;
        t.lifetime      = 28800;
        string tgt  = SimpleCrypto::encrypt(tgsKey, t.toString());
        string data = sk + "|" + tgsId + "|" + to_string(ts) + "|28800|" + tgt;
        return SimpleCrypto::encrypt(users[clientId], data);
    }
    string getKey(){ return tgsKey; }
};

class TGS{
    string key;
    unordered_map<string,string> serviceKeys;
public:
    TGS(string k){
        key = k;
        serviceKeys["fileserver"] = "FILE_SERVER_KEY_123";
        serviceKeys["mailserver"] = "MAIL_SERVER_KEY_456";
        serviceKeys["webserver"]  = "WEB_SERVER_KEY_789";
    }
    string request(string serviceId, string tgt, string auth, string tgsSk){
        if(serviceKeys.find(serviceId) == serviceKeys.end()) return "ERROR";
        string newSk = "SK_TGS_" + serviceId + "_" + to_string(time(0));
        Ticket t;
        t.sessionKey    = newSk;
        t.clientId      = "client";
        t.clientAddress = "192.168.1.100";
        t.serverId      = serviceId;
        t.timestamp     = time(0);
        t.lifetime      = 3600;
        string svcTicket = SimpleCrypto::encrypt(serviceKeys[serviceId], t.toString());
        string data      = newSk + "|" + serviceId + "|" + to_string(time(0)) + "|" + svcTicket;
        return SimpleCrypto::encrypt(tgsSk, data);
    }
    string getServiceKey(string id){
        auto it = serviceKeys.find(id);
        return (it != serviceKeys.end()) ? it->second : "";
    }
};

class ServiceServer{
    string id, masterKey;
public:
    ServiceServer(string i, string k): id(i), masterKey(k){}

    bool auth(string svcTicket, string auth, string &res){
        string ticketPlain = SimpleCrypto::decrypt(masterKey, svcTicket);
        string sk = extractField(ticketPlain, "Key:");
        if(sk.empty()){
            cout << "  [SRV] Failed to extract session key from ticket.\n";
            res = "ERROR";
            return false;
        }
        string authPlain = SimpleCrypto::decrypt(sk, auth);
        time_t ts = extractTime(authPlain);
        if(ts == 0 || abs(difftime(time(0), ts)) > 300){
            cout << "  [SRV] Authenticator timestamp invalid or expired.\n";
            res = "ERROR";
            return false;
        }
        res = SimpleCrypto::encrypt(sk, to_string(ts + 1));
        return true;
    }

private:
    string extractField(const string &s, const string &label){
        size_t p = s.find(label);
        if(p == string::npos) return "";
        p += label.size();
        size_t q = s.find_first_of(",]", p);
        if(q == string::npos) q = s.size();
        return s.substr(p, q - p);
    }
    time_t extractTime(const string &s){
        string val = extractField(s, "Time:");
        if(val.empty()) return 0;
        try { return (time_t)stol(val); }
        catch(...){ return 0; }
    }
};

class Client{
    string id, addr, pwd, tgsSk;
    unordered_map<string,string> tickets;
    unordered_map<string,string> svcSessionKeys;
public:
    Client(string i, string a, string p): id(i), addr(a), pwd(p){}

    vector<string> splitBy(const string &s, char delim){
        vector<string> v;
        stringstream ss(s);
        string t;
        while(getline(ss, t, delim)) v.push_back(t);
        return v;
    }

    bool getTGS(AuthenticationServer &as){
        string res = as.authenticateClient(id, "TGS", time(0));
        if(res == "ERROR") return false;
        string d = SimpleCrypto::decrypt(pwd, res);
        auto v = splitBy(d, '|');
        if(v.size() < 5) return false;
        tgsSk          = v[0];
        tickets["TGS"] = v[4];
        cout << "  [AS]  TGT obtained. Session key with TGS established.\n";
        return true;
    }

    bool getServiceTicket(TGS &tgs, const string &service){
        Authenticator a;
        a.clientId      = id;
        a.clientAddress = addr;
        a.timestamp     = time(0);
        string auth = SimpleCrypto::encrypt(tgsSk, a.toString());
        string res  = tgs.request(service, tickets["TGS"], auth, tgsSk);
        if(res == "ERROR") return false;
        string d = SimpleCrypto::decrypt(tgsSk, res);
        auto v = splitBy(d, '|');
        if(v.size() < 4) return false;
        svcSessionKeys[service] = v[0];
        tickets[service]        = v[3];
        cout << "  [TGS] Service ticket obtained for: " << service << "\n";
        return true;
    }

    bool accessService(ServiceServer &srv, const string &service){
        string sk = svcSessionKeys[service];
        Authenticator a;
        a.clientId      = id;
        a.clientAddress = addr;
        a.timestamp     = time(0);
        string auth = SimpleCrypto::encrypt(sk, a.toString());
        string res;
        bool ok = srv.auth(tickets[service], auth, res);
        if(ok){
            SimpleCrypto::decrypt(sk, res);
            cout << "  [SRV] Mutual authentication confirmed.\n";
        }
        return ok;
    }
};

int main(){
    cout << "===== Kerberos Authentication Protocol =====\n\n";

    AuthenticationServer as;
    TGS tgs(as.getKey());

    string user, pass, service;
    cout << "Enter username: ";  cin >> user;
    cout << "Enter password: ";  cin >> pass;

    Client c(user, "192.168.1.100", pass);

    cout << "\n[Step 1] Contacting Authentication Server...\n";
    if(!c.getTGS(as)){
        cout << "  Authentication Failed: Invalid username or password.\n";
        return 0;
    }

    cout << "\n[Step 2] Requesting service ticket...\n";
    cout << "  Available services: fileserver, mailserver, webserver\n";
    cout << "Enter service: ";  cin >> service;

    if(!c.getServiceTicket(tgs, service)){
        cout << "  Service Ticket Failed: Service not found.\n";
        return 0;
    }

    cout << "\n[Step 3] Accessing service...\n";
    ServiceServer srv(service, tgs.getServiceKey(service));
    if(c.accessService(srv, service)){
        cout << "\n  >>> Access Granted to [" << service << "] <<<\n";
    } else {
        cout << "\n  >>> Access Denied <<<\n";
    }
    return 0;
}
/*
===== Kerberos Authentication Protocol =====

Enter username: alice
Enter password: password123

[Step 1] Contacting Authentication Server...
  [AS]  TGT obtained. Session key with TGS established.

[Step 2] Requesting service ticket...
  Available services: fileserver, mailserver, webserver
Enter service: webserver
  [TGS] Service ticket obtained for: webserver

[Step 3] Accessing service...
  [SRV] Mutual authentication confirmed.

  >>> Access Granted to [webserver] <<<
PS C:\Users\khushbu\OneDrive\Desktop\Cryptography and Network Security\Extras>
*/