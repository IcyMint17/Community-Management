#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip> 
using namespace std;

//小区资料
struct Community {
    string name;       // 小区名称
    int buildingCount; // 楼宇总数
    vector<int> buildingNumbers; // 楼宇编号列表
};
//房型资料
struct HouseType {
    string id;           // 房型编号
    float area;          // 房型面积
    float fee;            // 收费标准
};
//住户资料
struct Resident {
    string id;                // 住户编号
    string houseId;        // 住户房型编号
    string name;           // 住户姓名
    string phone;          // 联系电话
    string address;        // 联系地址
    string password;       // 住户密码
    float prepaidAmount;   //应付金额
    float unpaidAmount;    // 欠费金额
};
//住户报修管理与投诉管理
struct Complaint {
    string residentId;        // 投诉住户编号
    string description;    // 投诉内容
    bool resolved;         // 是否已解决
};
struct Repair {
    string residentId;        // 报修住户编号
    string description;    // 报修内容
    bool resolved;         // 是否已解决
};
//住户物业收费管理
struct PropertyFee {
    int id;
    string name;
    float amount;          // 收费金额
    string unit;           // 收费单位（如元/月，元/季，元/年）
    string period;         // 收费周期
};

struct Community communities;
vector<HouseType> houseTypes;
vector<Resident> residents;     //住户数据
vector<Complaint> complaints;
vector<Repair> repairs;
vector<PropertyFee> propertyfees;

vector<Resident> unpaidResidents;//未交物业费住户列表

string adminPassword = "admin123"; // 管理员密码

void adminMenu(void);
void userMenu(int residentId);
bool Login(string role);

void addResident(void);
void modifyResident(void);
void viewResident(void);
void deleteResident(void);

void loadfromFile(const string& filename);
void savetoFile(const string &filename);

void modifyCommunity(void);
void viewCommunity(void);

void addHousetype(void);
void modifiyHousetype(void);
void deleteHousetype(void);
void viewHousetype(void);
void countHousetype(void);

void printRepair(Repair repair);
void printComplaint(Complaint complaint);
void printHouseType(HouseType houseType);
void printResident(Resident resident);

void addComplaints(string role, string userid);
void deleteComplaints(void);
void modifiyComplaints(void);
void viewComplaints(void);

void addRepairs(string role, string userid);
void modifiyRepairs(void);
void deleteRepairs(void);
void viewRepairs(void);

void findUnpaidResident(void);
void sortUnpaid();
void remindUser(string userid);

void printFee(PropertyFee fee);
void viewFee();
void deleteFee();
void modifiyFee();
void addFee();


//从文件读取
void loadfromFile(const string& filename) {
    vector<string> fileList = {
        "./data/residents.txt",
        "./data/community.txt",
        "./data/housetype.txt",
        "./data/complaints.txt",
        "./data/repairs.txt",
        "./data/fees.txt"
    };
    //住户 、小区 、房型 、投诉、 报修、费用
    int fileindex;
    if (filename == "resident") fileindex = 0;
    else if (filename == "community") fileindex = 1;
    else if (filename == "housetype") fileindex = 2;
    else if (filename == "complaint") fileindex = 3;
    else if (filename == "repair") fileindex = 4;
    else if (filename == "fee") fileindex = 5;
    else fileindex = -1;
    
    if (fileindex != -1) {
        ifstream file(fileList[fileindex]);//打开文件
        if (!file) {            //检查文件是否成功打开
            cout << "无法打开文件:" << fileList[fileindex] << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            switch (fileindex)
            {
            case 0:
            {
                Resident resident;
                // 解析一行数据，根据文件中逗号分隔的格式
                string prepaidAmount, unpaidAmount;
                getline(ss, resident.id, ',');               // 读取住户编号
                getline(ss, resident.name, ',');    // 读取住户姓名
                getline(ss, resident.phone, ',');   // 读取联系电话
                getline(ss, resident.address, ','); // 读取联系地址
                getline(ss, prepaidAmount, ',');    // 读取应付金额
                getline(ss, unpaidAmount, ',');     // 读取欠费金额
                getline(ss, resident.houseId, ',');          // 读取住户房型
                getline(ss, resident.password, ',');
                resident.prepaidAmount = stof(prepaidAmount);
                resident.unpaidAmount = stof(unpaidAmount);
                residents.push_back(resident);
                break;
            }
            case 1: // 读取小区信息        
            {
                string buildingCount;
                getline(ss, communities.name, ',');
                getline(ss, buildingCount, ',');
                communities.buildingCount = stoi(buildingCount);
                break;
            }
            case 2: // 读取房型信息       
            {
                HouseType houseType;
                string area, fee;
                getline(ss, houseType.id, ',');
                getline(ss, area, ',');
                getline(ss, fee, ',');
                houseType.area = stof(area);
                houseType.fee = stof(fee);
                houseTypes.push_back(houseType);
                break;
            }
            case 3: // 读取投诉信息
            {
                Complaint complaint;
                string isresolved;
                getline(ss, complaint.residentId, ',');
                getline(ss, complaint.description, ',');
                getline(ss, isresolved, ',');
                complaint.resolved = (isresolved == "true") ? true : false;
                complaints.push_back(complaint);
                break;
            }
            case 4: // 读取报修信息
            {
                Repair repair;
                string isresolved;
                getline(ss, repair.residentId, ',');
                getline(ss, repair.description, ',');
                getline(ss, isresolved, ',');
                repair.resolved = (isresolved == "true") ? true : false;
                repairs.push_back(repair);
                break;
            }
            case 5: // 读取费用信息
            {
                PropertyFee fee;
                string id, amount;
                getline(ss, id, ',');
                getline(ss, fee.name, ',');
                getline(ss, amount, ',');
                getline(ss, fee.unit, ',');
                getline(ss, fee.period, ',');
                fee.id = stoi(id);
                fee.amount = stof(amount);
                propertyfees.push_back(fee);
                //cout << "费用加载" << endl;

                break;
            }
            default:break;
            }
        }
        cout << "文件 " << fileList[fileindex] << " 加载成功！" << endl;
        file.close();
    }
}
//保存到文件
void savetoFile(const string &filename) {
    vector<string> fileList = {
        "./data/residents.txt",
        "./data/community.txt",
        "./data/housetype.txt",
        "./data/complaints.txt",
        "./data/repairs.txt",
        "./data/fees.txt"
    };
    int fileindex;
    if (filename == "resident") fileindex = 0;
    else if (filename == "community") fileindex = 1;
    else if (filename == "housetype") fileindex = 2;
    else if (filename == "complaint") fileindex = 3;
    else if (filename == "repair") fileindex = 4;
    else if (filename == "fee") fileindex = 5;
    else fileindex = -1;

    if (fileindex != -1) {
        ofstream file(fileList[fileindex]);  // ios::app 可以用于追加
        if (!file) {  // 检查文件是否成功打开
            cout << "无法打开文件" << endl;
            return;
        }

        switch (fileindex) {
        case 0:  // residents
        {
            for (const auto& resident : residents) {
                file << resident.id << ","
                    << resident.name << ","
                    << resident.phone << ","
                    << resident.address << ","
                    << resident.prepaidAmount << ","
                    << resident.unpaidAmount << ","
                    << resident.houseId<<","
                    << resident.password <<endl;
            }
            break;  // 这里的 break 保留，因为是 switch 的一部分
        }
        case 1:  // communities
        {
            file << communities.name << ","
                << communities.buildingCount << endl;
            break;
        }
        case 2:  // houseTypes
        {
            for (const auto& housetype : houseTypes) {
                file << housetype.id << ","
                    << housetype.area << ","
                    << housetype.fee << endl;
            }
            break;
        }
        case 3:  // complaints
        {
            for (const auto& complaint : complaints) {
                file << complaint.residentId << ","
                    << complaint.description << ","
                    << complaint.resolved << endl;
            }
            break;
        }
        case 4:  // repairs
        {
            for (const auto& repair : repairs) {
                file << repair.residentId << ","
                    << repair.description << ","
                    << repair.resolved << endl;
            }
            break;
        }
        case 5:
        {
            for (const auto& fee : propertyfees) {
                file << fee.id << ","
                    << fee.name << ","
                    << fee.amount << ","
                    << fee.unit << ","
                    << fee.period << endl;
            }
            break;
        }
        default: break;
        }

        file.close();  // 关闭文件
        cout << "文件已保存" << endl;
    }
}


// 打印住户信息
void printResident(Resident resident) {
    cout << setw(10) << resident.id << "\t";
    cout << setw(5) << resident.name << "\t";
    cout << setw(15) << resident.phone << "\t";
    cout << setw(10) << resident.address << "\t";
    cout << setw(3) << resident.houseId << "\t";
    cout << setw(8) << resident.prepaidAmount << "\t";
    cout << setw(8) << resident.unpaidAmount << "\t";
    cout << setw(8) << resident.password << endl;
}

// 打印房型信息
void printHouseType(HouseType houseType) {
    cout << setw(3) << houseType.id << "\t";
    cout << setw(5) << houseType.area << "\t";
    cout << setw(5) << houseType.fee << endl;

}

// 打印投诉信息
void printComplaint(Complaint complaint) {
    cout << setw(10) << complaint.residentId << "\t";
    cout << setw(20) << complaint.description << "\t";
    cout << setw(4) << (complaint.resolved ? "是" : "否") << endl;

}

// 打印报修信息
void printRepair(Repair repair) {
    cout << setw(10) << repair.residentId << "\t";
    cout << setw(20) << repair.description << "\t";
    cout << setw(4) << (repair.resolved ? "是" : "否") << endl;
}


void printFee(PropertyFee fee) {
    cout << setw(3) << fee.id << "\t";
    cout << setw(10) << fee.name << "\t";
    cout << setw(5) << fee.amount << "\t";
    cout << setw(4) << fee.unit << "\t";
    cout << setw(4) << fee.period << endl;
}

//查找住户
void findResident() {
    string id;
    cout << "输入要查找的住户id:" << endl;
    cin >> id;

    for (const auto& resident : residents) {
        if (resident.id == id) {
            cout << setw(10) << "\033[33m住户编号" << "\t";
            cout << setw(5) << "姓名" << "\t";
            cout << setw(15) << "电话" << "\t";
            cout << setw(10) << "住址" << "\t";
            cout << setw(3) << "房型" << "\t";
            cout << setw(8) << "待付金额" << "\t";
            cout << setw(8) << "未付金额" << "\t";
            cout << setw(8) << "用户密码\033[0m" << endl;
            printResident(resident);
            system("pause");
            return;
        }
    }
    cout << "id输入有误或住户不存在！" << endl;
    system("pause");

}

//新增住户
void addResident() {
    Resident resident;
    cout << "\033c";
    cout << "********新增住户********:" << endl;
    cout << "输入住户编号:" << endl;
    cin >> resident.id;
    cout << "输入住户姓名:" << endl;
    cin >> resident.name;
    cout << "输入联系电话:" << endl;
    cin >> resident.phone;
    cout << "输入联系地址:" << endl;
    cin >> resident.address;
    cout << "输入住户户型:" << endl;
    cin >> resident.houseId;
    cout << "输入住户密码:" << endl;
    cin >> resident.password;
    resident.prepaidAmount = 0;
    resident.unpaidAmount = 0;
    //将新增住户添加到总住户中
    residents.push_back(resident);
    cout << "住户添加成功！" << endl;
    savetoFile("resident");
    system("pause");
}

//修改住户
void modifyResident() {
    string id;
    cout << "输入要修改的住户id:" << endl;
    cin >> id;

    for (auto& resident : residents) {
        if (resident.id == id) {
            cout << "输入住户姓名:" << endl;
            cin >> resident.name;
            cout << "输入联系电话:" << endl;
            cin >> resident.phone;
            cout << "输入联系地址:" << endl;
            cin >> resident.address;
            cout << "输入住户户型:" << endl;
            cin >> resident.houseId;
            cout << "输入住户密码:" << endl;
            cin >> resident.password;
            savetoFile("resident");
            cout << "修改成功！" << endl;

            system("pause");

            return;
        }
    }
    cout << "id输入有误或住户不存在！" << endl;
    system("pause");

}

//查看住户
void viewResident() {
    if (residents.size() == 0) {
        cout<<"住户列表为空！" << endl;
    }
    else {
        cout << setw(10) << "\033[33m住户编号" << "\t";
        cout << setw(5) << "姓名" << "\t";
        cout << setw(15) << "电话" << "\t";
        cout << setw(10) << "住址" << "\t";
        cout << setw(3) << "房型" << "\t";
        cout << setw(8) << "待付金额" << "\t"; 
        cout << setw(8) << "未付金额" << "\t";
        cout << setw(8) << "用户密码\033[0m" << endl;
        for (const auto& resident : residents) {
            printResident(resident);
        }
        
    }
    
    system("pause");
}

//删除住户
void deleteResident() {
    string id;
    cout << "输入要删除的住户id:" << endl;
    cin >> id;

    int cont = 0;
    for (const auto& resident : residents) {
        if (resident.id == id) {
            cout << "已找到住户：" << endl;
            cout << setw(10) << "\033[33m住户编号" << "\t";
            cout << setw(5) << "姓名" << "\t";
            cout << setw(15) << "电话" << "\t";
            cout << setw(10) << "住址" << "\t";
            cout << setw(3) << "房型" << "\t";
            cout << setw(8) << "待付金额" << "\t";
            cout << setw(8) << "未付金额" << "\t";
            cout << setw(8) << "用户密码\033[0m" << endl;
            printResident(resident);
            cout << "是否删除该住户？(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                residents.erase(residents.begin() + cont);
                savetoFile("resident");

            }
            else {
                cout << "取消操作" << endl;
            }
            system("pause");
            return;
        }
        cont++;
    }
    cout << "id输入有误或住户不存在！" << endl;
    system("pause");

}

//修改小区资料
void modifyCommunity() {
    cout << "\033c";
    cout << "输入小区名称:" << endl;
    cin >> communities.name;
    cout << "输入楼宇总数:" << endl;
    cin >> communities.buildingCount;
    communities.buildingNumbers.clear();
    for (int i = 1; i < communities.buildingCount + 1; i++) {
        communities.buildingNumbers.push_back(i);
    }
    savetoFile("community");

    system("pause");

}
//查看小区资料
void viewCommunity() {
    cout << "\033c";
    cout <<"小区名称：" << communities.name << endl;
    cout <<"楼房总数：" << communities.buildingCount << endl;
    system("pause");

}

//添加房型
void addHousetype() {
    HouseType houseType;
    cout << "********新增住户********:" << endl;
    cout << "输入房型编号:" << endl;
    cin >> houseType.id;
    cout << "输入房型面积:" << endl;
    cin >> houseType.area;
    cout << "输入房型收费标准:" << endl;
    cin >> houseType.fee;
    //将新增住户添加到总住户中
    houseTypes.push_back(houseType);
    savetoFile("housetype");
    cout << "房型添加成功！" << endl;
    system("pause");

}

//修改房型
void modifiyHousetype() {
    string id;
    cout << "输入要修改的房型编号:" << endl;
    cin >> id;
    for (auto& housetype : houseTypes) {
        if (housetype.id == id) {
            cout << "修改房型面积:" << endl;
            cin >> housetype.area;
            cout << "修改房型收费标准:" << endl;
            cin >> housetype.fee;
            savetoFile("housetype");
            cout << "房型已修改！" << endl;
            system("pause");

            return;
        }
    }
    cout << "输入有误或房型不存在！" << endl;
    system("pause");
}

//删除房型
void deleteHousetype() {
    string id;
    cout << "输入要删除的房型编号:" << endl;
    cin >> id;

    int cont = 0;
    for (const auto& housetype : houseTypes) {
        if (housetype.id == id) {
            cout << "已找到房型：" << endl;
            cout << setw(3) << "\033[33m编号" << "\t";
            cout << setw(5) << "面积" << "\t";
            cout << setw(5) << "收费标准\033[0m" << endl;
            printHouseType(housetype);
            cout << "\n\n是否删除该房型？(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                houseTypes.erase(houseTypes.begin() + cont);
                savetoFile("housetype");
            }
            else {
                cout << "取消操作" << endl;
            }
            return;
        }
        cont++;
    }
    cout << "输入有误或房型不存在！" << endl;
    system("pause");
}

//查看所有房型
void viewHousetype() {
    if (houseTypes.size() == 0) {
        cout << "房型列表为空" << endl;
    }
    else {
        cout << setw(3) << "\033[33m编号" << "\t";
        cout << setw(5) << "面积" << "\t";
        cout << setw(5) << "收费标准\033[0m" << endl;
        for (const auto& housetype : houseTypes) {
            printHouseType(housetype);
        }
    }

    system("pause");
}

//查找房型
void findHousetype() {
    string id;
    cout << "输入要查找的房型编号:" << endl;
    cin >> id;

    for (const auto& housetype : houseTypes) {
        if (housetype.id == id) {
            cout << setw(3) << "\033[33m编号" << "\t";
            cout << setw(5) << "面积" << "\t";
            cout << setw(5) << "收费标准\033[0m" << endl;
            printHouseType(housetype);
            system("pause");
            return;
        }
    }
    cout << "输入有误或房型不存在！" << endl;
    system("pause");

}

//房型统计
void countHousetype() {
    struct Counthouse {
        string id;         //房型id
        int count = 0;      //房型总数
    };
    vector<Counthouse> counthouse(houseTypes.size()); //根据房型个数创建容器大小
    int i = 0;
    for (const auto& housetype : houseTypes) {          //遍历房型列表，将每个房型id存入新结构体counthouse中用来统计
        counthouse[i].id = housetype.id;
        i++;
    }
    for (const auto& resident : residents) {
        for (auto& count : counthouse) {
            if (resident.houseId == count.id) {
                count.count++;
            }
        }
    }
    //显示统计结果
    for (auto& count : counthouse) {
        cout << "房型：" << count.id << " 共：" << count.count <<" 户" << endl;
    }
    system("pause");
}

//投诉新建
void addComplaints(string role, string userid) {
    Complaint complaint;
    if (role == "admin") {
        cout << "\033c";
        cout << "********新增投诉信息********:" << endl;
        cout << "输入投诉住户编号:" << endl;
        cin >> complaint.residentId;
        cout << "输入投诉内容:" << endl;
        cin >> complaint.description;
        char option;
        cout << "是否已解决(Y/N):" << endl;
        cin >> option;
        if (option == 'Y') {
            complaint.resolved = true;
        }
        else if (option == 'N') {
            complaint.resolved = false;
        }
        else {
            cout << "输入有误！" << endl;

        }
        //将投诉信息添加到总投诉信息中               
    }
    else if (role == "user") {
        cout << "\033c";
        cout << "********新增投诉信息********:" << endl;
        complaint.residentId = userid;
        cout << "输入投诉内容:" << endl;
        cin >> complaint.description;
        complaint.resolved = false;
    }
    complaints.push_back(complaint);
    savetoFile("complaint");
    cout << "投诉信息添加成功！" << endl;
}
//投诉修改
void modifiyComplaints() {
    string id;
    cout << "输入要修改的投诉住户编号:" << endl;
    cin >> id;

    for (auto& complaint : complaints) {
        if (complaint.residentId == id) {
            cout << "修改投诉内容:" << endl;
            cin >> complaint.description;
            char option;
            cout << "是否已解决(Y/N):" << endl;
            cin >> option;
            if (option == 'Y') {
                complaint.resolved = true;
            }
            else if (option == 'N') {
                complaint.resolved = false;
            }
            else {
                cout << "输入有误！" << endl;
            }
            savetoFile("complaint");

            system("pause");
            return;
        }
    }
    cout << "输入有误或房型不存在！" << endl;
    system("pause");
}

//投诉删除
void deleteComplaints() {
    string id;
    cout << "输入要删除的投诉住户编号:" << endl;
    cin >> id;

    int cont = 0;
    for (auto& complaint : complaints) {
        if (complaint.residentId == id) {
            cout << "已找到住户：" << endl;
            cout << "住户编号：" << complaint.residentId << endl;
            cout << "是否删除该住户投诉信息？(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                complaints.erase(complaints.begin() + cont);
                savetoFile("complaint");

            }
            else {
                cout << "取消操作" << endl;
            }
            system("pause");
            return;
        }
        cont++;
    }
    cout << "输入有误" << endl;
    system("pause");
}

//查看投诉
void viewComplaints() {
    if (complaints.size() == 0) {
        cout << "投诉列表为空" << endl;
    }
    else {
        cout << setw(10) << "\033[33m投诉编号" << "\t";
        cout << setw(20) << "投诉内容" << "\t";
        cout << setw(4) << "是否解决\033[0m" << endl;
        for (const auto& complaint : complaints) {
            printComplaint(complaint);
        }
    }

    system("pause");
}

//查找投诉
void findComplaints() {
    string id;
    cout << "输入要查找的投诉住户编号:" << endl;
    cin >> id;

    for (auto& complaint : complaints) {
        if (complaint.residentId == id) {
            cout << setw(10) << "\033[33m投诉编号" << "\t";
            cout << setw(20) << "投诉内容" << "\t";
            cout << setw(4) << "是否解决\033[0m" << endl;
            printComplaint(complaint);
            system("pause");
            return;
        }
    }
    cout << "输入有误或房型不存在！" << endl;
    system("pause");
}

//报修新建
void addRepairs(string role, string userid) {
    Repair repair;
    if (role == "admin") {
        cout << "\033c";
        cout << "********新增报修信息********:" << endl;
        cout << "输入报修住户编号:" << endl;
        cin >> repair.residentId;
        cout << "输入报修内容:" << endl;
        cin >> repair.description;
        char option;
        cout << "是否已解决(Y/N):" << endl;
        cin >> option;
        if (option == 'Y') {
            repair.resolved = true;
        }
        else if (option == 'N') {
            repair.resolved = false;
        }
        else {
            cout << "输入有误！" << endl;

        }
    }
    else if (role == "admin") {
        repair.residentId = userid;
        cout << "输入报修内容:" << endl;
        cin >> repair.description;
        repair.resolved = false;
    }
    //将报修信息添加到总报修信息中
    repairs.push_back(repair);
    savetoFile("repair");

    cout << "报修信息添加成功！" << endl;
    system("pause");
}

//报修修改
void modifiyRepairs() {
    string id;
    cout << "输入要修改的报修住户编号:" << endl;
    cin >> id;

    for (auto& repair : repairs) {
        if (repair.residentId == id) {
            cout << "修改报修内容:" << endl;
            cin >> repair.description;
            char option;
            cout << "是否已解决(Y/N):" << endl;
            cin >> option;
            if (option == 'Y') {
                repair.resolved = true;
            }
            else if (option == 'N') {
                repair.resolved = false;
            }
            else {
                cout << "输入有误！" << endl;
            }
            savetoFile("repair");

            system("pause");
            return;
        }
    }
    cout << "输入有误或房型不存在！" << endl;
    system("pause");
}

//报修删除
void deleteRepairs() {
    string id;
    cout << "输入要删除的报修住户编号:" << endl;
    cin >> id;

    int cont = 0;
    for (auto& repair : repairs) {
        if (repair.residentId == id) {
            cout << "已找到住户：" << endl;
            cout << "住户编号：" << repair.residentId << endl;
            cout << "是否删除该住户报修信息？(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                repairs.erase(repairs.begin() + cont);
                savetoFile("repair");

            }
            else {
                cout << "取消操作" << endl;
            }
            system("pause");
            return;
        }
        cont++;
    }
    cout << "输入有误" << endl;
    system("pause");
}

//查看报修
void viewRepairs() {
    if (repairs.size() == 0) {
        cout << "投诉列表为空" << endl;
    }
    else {
        cout << setw(10) << "\033[33m报修编号" << "\t";
        cout << setw(20) << "报修内容" << "\t";
        cout << setw(4) << "是否解决\033[0m" << endl;
        for (const auto& repair : repairs) {
            printRepair(repair);
        }
    }
    system("pause");
}

//查找报修
void findRepairs() {
    string id;
    cout << "输入要查找的报修住户编号:" << endl;
    cin >> id;

    for (auto& repair : repairs) {
        if (repair.residentId == id) {
            cout << setw(10) << "\033[33m报修编号" << "\t";
            cout << setw(20) << "报修内容" << "\t";
            cout << setw(4) << "是否解决\033[0m" << endl;
            printRepair(repair);
            system("pause");
            return;
        }
    }
    cout << "输入有误或房型不存在！" << endl;
    system("pause");
}



//查找欠费住户
void findUnpaidResident() {
    unpaidResidents.clear();
    for (const auto& resident : residents) {
        if (resident.unpaidAmount > 0) {

            unpaidResidents.push_back(resident);
        }
    }
    sortUnpaid();
    cout << "欠款住户：（按欠款金额排序）" << endl;
    cout << setw(10) << "\033[33m住户编号" << "\t";
    cout << setw(5) << "姓名" << "\t";
    cout << setw(15) << "电话" << "\t";
    cout << setw(10) << "住址" << "\t";
    cout << setw(8) << "待付金额" << "\t";
    cout << setw(8) << "未付金额\033[0m" << endl;
    for (const auto& resident : unpaidResidents) {
        cout << setw(10) << resident.id << "\t";
        cout << setw(5) << resident.name << "\t";
        cout << setw(15) << resident.phone << "\t";
        cout << setw(10) << resident.address << "\t";
        cout << setw(8) << resident.prepaidAmount << "\t";
        cout << setw(8) << resident.unpaidAmount << endl;
    }
    system("pause");
}


//欠款住户排序
void sortUnpaid() {
    
    sort(unpaidResidents.begin(), unpaidResidents.end(), [](const Resident& a, const Resident& b) {
        return a.unpaidAmount > b.unpaidAmount; });  // 欠费最多的住户排在前面
}

//计算物业费
void calcFee() {
    
    for (auto& resident : residents) {
        float feeAmount = 0.0;
        for (const auto& housetype : houseTypes) {
            if (housetype.id == resident.houseId) {
                feeAmount = float(housetype.area * housetype.fee);
            }
        }
        for (const auto& fee : propertyfees) {
            feeAmount += fee.amount;
        }
        resident.prepaidAmount = feeAmount;
    }
    cout << "物业费已计算完成。" << endl;
    savetoFile("resident");
    system("pause");
}



void addFee() {
    PropertyFee fee;
    cout << "********新增收费********:" << endl;
    cout << "输入收费编号:" << endl;
    cin >> fee.id;
    cout << "输入收费名称:" << endl;
    cin >> fee.name;
    cout << "输入收费金额:" << endl;
    cin >> fee.amount;
    cout << "输入收费单位:" << endl;
    cin >> fee.unit;
    cout << "输入收费周期:" << endl;
    cin >> fee.period;
    propertyfees.push_back(fee);
    savetoFile("fee");
    cout << "收费添加成功！" << endl;
    system("pause");

}

//修改fee
void modifiyFee() {
    int id;
    cout << "输入要修改的收费编号:" << endl;
    cin >> id;
    for (auto& fee : propertyfees) {
        if (fee.id == id) {
            cout << "输入收费编号:" << endl;
            cin >> fee.id;
            cout << "输入收费名称:" << endl;
            cin >> fee.name;
            cout << "输入收费金额:" << endl;
            cin >> fee.amount;
            cout << "输入收费单位:" << endl;
            cin >> fee.unit;
            cout << "输入收费周期:" << endl;
            cin >> fee.period;
            savetoFile("fee");
            cout << "收费已修改！" << endl;
            system("pause");

            return;
        }
    }
    cout << "输入有误或费用不存在！" << endl;
    system("pause");
}

//删除fee
void deleteFee() {
    int id;
    cout << "输入要删除的收费编号:" << endl;
    cin >> id;

    int cont = 0;
    for (const auto& fee : propertyfees) {
        if (fee.id == id) {
            cout << "已找到收费：" << endl;
            printFee(fee);
            cout << "\n\n是否删除该收费？(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                propertyfees.erase(propertyfees.begin() + cont);
                savetoFile("fee");
            }
            else {
                cout << "取消操作" << endl;
            }
            return;
        }
        cont++;
    }
    cout << "输入有误或费用不存在！" << endl;
    system("pause");
}

//查看收费
void viewFee() {
    if (propertyfees.size() == 0) 
    {
        cout << "费用列表为空" << endl;
    }
    else 
    {
        cout << setw(3) << "\033[33m编号" << "\t";
        cout << setw(10) << "费用名称" << "\t";
        cout << setw(5) << "金额" << "\t";
        cout << setw(4) << "单位" << "\t";
        cout << setw(4) << "缴费周期\033[0m" << endl;
        for (const auto& fee : propertyfees) {
            printFee(fee);
        }
    }
    system("pause");
}




//更改密码
void changePassWord(string resi) {
    for (auto& resident : residents) {
        if (resident.id == resi) {
            cout << "输入新密码:" << endl;
            cin >> resident.password;
        }
    }
    savetoFile("resident");
    system("pause");
}
//管理员菜单
void adminMenu() {
    int choice;
    do {
        cout << "\033c";
        cout << "\033[33m*******************\033[5m  欢迎使用小区物业管理系统！\033[0m\033[33m*********************\033[0m" << endl;
        cout << "                                                                  " << endl;
        cout << "                           管理员菜单                               " << endl;
        cout << "                                                                  " << endl;
        cout << "                           1.小区管理                                 "<< endl;
        cout << "                           2.房型管理                                 " << endl;
        cout << "                           3.住户管理                                 " << endl;
        cout << "                           4.报修管理                             " << endl;
        cout << "                           5.投诉管理                              " << endl;
        cout << "                           6.物业收费管理                          " << endl;
        cout << "                           0:退出                                     " << endl;
        cout << "                                                                  " << endl;
        cout << "\033[33m********************************************************************\033[0m" << endl;
        cout << "\033[32m请选择操作\033[0m：" << endl;
        cin >> choice;
        int choice_next = 0;
        do
        {
            switch (choice)
            {
            case 1:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  欢迎使用小区物业管理系统！\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      管理员菜单>小区资料                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                       1.修改资料                                  " << endl;
                cout << "                       2.查看资料                                  " << endl;
                cout << "                       0:退出                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m请选择操作\033[0m：" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:modifyCommunity(); break;
                case 2:viewCommunity(); break;
                case 0:break;
                default:
                    cout << "无效选项!" << endl;
                    break;
                }
                break;
            case 2:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  欢迎使用小区物业管理系统！\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      管理员菜单>房型管理                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.新增房型                                  " << endl;
                cout << "                        2.修改房型                                  " << endl;
                cout << "                        3.删除房型                                  " << endl;
                cout << "                        4.查看房型                                  " << endl;
                cout << "                        5.房型统计                                  " << endl;
                cout << "                        6.查找房型                                  " << endl;
                cout << "                        0:退出                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m请选择操作\033[0m：" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:addHousetype(); break;
                case 2:modifiyHousetype(); break;
                case 3:deleteHousetype(); break;
                case 4:viewHousetype(); break;
                case 5:countHousetype(); break;
                case 6:findHousetype(); break;

                case 0:break;
                default:
                    cout << "无效选项!" << endl;
                    break;
                }
                break;
            case 3:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  欢迎使用小区物业管理系统！\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      管理员菜单>住户管理                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.新增住户                                  " << endl;
                cout << "                        2.修改住户                                  " << endl;
                cout << "                        3.删除住户                                  " << endl;
                cout << "                        4.查看住户                                  " << endl;
                cout << "                        5.查找住户                                  " << endl;
                cout << "                        0:退出                                     " << endl;
                cout << "                                                                   " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m请选择操作\033[0m：" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:addResident(); break;
                case 2:modifyResident(); break;
                case 3:deleteResident(); break;
                case 4:viewResident(); break;
                case 5:findResident(); break;

                case 0:break;
                default:
                    cout << "无效选项!" << endl;
                    break;
                }
                break;
            case 4:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  欢迎使用小区物业管理系统！\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      管理员菜单>报修管理                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.新增报修                                  " << endl;
                cout << "                        2.修改报修                                  " << endl;
                cout << "                        3.删除报修                                  " << endl;
                cout << "                        4.查看报修                                  " << endl;
                cout << "                        5.查找报修                                  " << endl;
                cout << "                        0:退出                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m请选择操作\033[0m：" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:addRepairs("admin", "0"); break;
                case 2:modifiyRepairs(); break;
                case 3:deleteRepairs(); break;
                case 4:viewRepairs(); break;
                case 5:findRepairs(); break;
                case 0:break;
                default:
                    cout << "无效选项!" << endl;
                    break;
                }
                break;
            case 5:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  欢迎使用小区物业管理系统！\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      管理员菜单>投诉管理                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.新增投诉                                  " << endl;
                cout << "                        2.修改投诉                                  " << endl;
                cout << "                        3.删除投诉                                  " << endl;
                cout << "                        4.查看投诉                                  " << endl;
                cout << "                        5.查找投诉                                  " << endl;
                cout << "                        0:退出                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m请选择操作\033[0m：" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:addComplaints("admin", "0"); break;
                case 2:modifiyComplaints(); break;
                case 3:deleteComplaints(); break;
                case 4:viewComplaints(); break;
                case 5:findComplaints(); break;
                case 0:break;
                default:
                    cout << "无效选项!" << endl;
                    break;
                }
                break;
            case 6:
            {
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  欢迎使用小区物业管理系统！\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                  管理员菜单>住户物业收费管理                       " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.查找未缴费住户                            " << endl;
                cout << "                        2.计算住户物业费                                " << endl;
                cout << "                        3.提醒未缴费住户                            " << endl;
                cout << "                        4.查看收费                            " << endl;
                cout << "                        5.修改收费                            " << endl;
                cout << "                        6.删除收费                            " << endl;
                cout << "                        7.新增收费                            " << endl;
                cout << "                        0:退出                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "请选择操作：" << endl;

                cin >> choice_next;
                switch (choice_next)
                {
                case 1:findUnpaidResident(); break;
                case 2:calcFee();break;
                case 3:
                {
                    cout << "提醒已发送到住户！：";
                    system("pause");
                    break;
                }
                case 4:viewFee(); break;
                case 5:modifiyFee(); break;
                case 6:deleteFee(); break;
                case 7:addFee(); break;
                case 0:break;
                default:
                    break;
                }
            }
            default:

                break;
            }
            } while (choice_next != 0);
        } while (choice != 0);
    
}

//住户菜单
void userMenu(string residentId) {
    int choice;
    do {
        cout << "\033c";
        remindUser(residentId);
        cout << "\033[33m*******************\033[5m  欢迎使用小区物业管理系统！\033[0m\033[33m*********************\033[0m" << endl;
        cout << "                                                                  " << endl;
        cout << "                             住户菜单                                " << endl;
        cout << "                                                                  " << endl;
        cout << "                             1.提交投诉                                  " << endl;
        cout << "                             2.查看投诉                                 " << endl;
        cout << "                             3.提交报修                                  " << endl; 
        cout << "                             4.查看报修                                  " << endl;
        cout << "                             5.修改密码                                  " << endl;
        cout << "                             0:退出                                     " << endl;
        cout << "                                                                  " << endl;
        cout << "\033[33m********************************************************************\033[0m" << endl;
        cout << "请选择操作：";

        cin >> choice;

        switch (choice)
        {
        case 1:addComplaints("user", residentId);
            break;
        case 2:
        {
            for (const auto& complaint : complaints) {
                if (complaint.residentId == residentId) {
                    cout << setw(10) << "\033[33m投诉编号" << "\t";
                    cout << setw(20) << "投诉内容" << "\t";
                    cout << setw(4) << "是否解决\033[0m" << endl;
                    printComplaint(complaint);
                }
            }
            system("pause");
        }
            break;
        case 3:addRepairs("user", residentId);
            break;
        case 4:
        {
            for (const auto& repair : repairs) {
                if (repair.residentId == residentId) {
                    cout << setw(10) << "\033[33m报修编号" << "\t";
                    cout << setw(20) << "报修内容" << "\t";
                    cout << setw(4) << "是否解决\033[0m" << endl;
                    printRepair(repair);
                }
            }
            system("pause");
           
        }
        break;
        case 5:
            changePassWord(residentId);
            break;
        
        default:
            break;
        }
    } while (choice != 0);
}
//提醒住户缴费
void remindUser(string userid) {
    for (const auto& resident : residents) {
        if (resident.id == userid && resident.unpaidAmount > 0) {
            cout << "\n\n" << "您还有未缴的物业费 " << resident.unpaidAmount << "元！\n" << endl;
        }
    }
}


bool Login(string role, string userid) {
    string inputPassword;
    int wrongtime=0;
    while (wrongtime < 3) {
        if (role == "admin") {
            cout << "请输入管理员密码:" << endl;
            cin >> inputPassword;
            if (inputPassword == adminPassword) {
                return true;
            }
            else {
                cout << "登录失败，密码错误！" << endl;
                wrongtime++;
            }

        }
        else if (role == "user") {

            for (const auto& resident : residents) {

                if (resident.id == userid) {
                    // 找到住户，开始验证密码
                    cout << "请输入住户" << userid << "的密码:" << endl;
                    cin >> inputPassword;
                    if (inputPassword == resident.password) {
                        cout << "登录成功！" << endl;
                        return true;

                    }
                    else {
                        cout << "登录失败，密码错误！" << endl;
                        wrongtime++;
                        
                    }
                }


            }
            cout << "没有该住户！" << endl;
            system("pause");
            return false;
            
        }
    }
    return false;
}


//主函数
int main() {
    
    loadfromFile("resident");
    loadfromFile("community");
    loadfromFile("housetype");
    loadfromFile("complaint");
    loadfromFile("repair");
    loadfromFile("fee");
    int option = 0;
    do {
        cout << "\033c";

        cout << "\033[33m===============小区物业管理系统登录================\033[0m\n" << endl;
        cout << "\033[32m                 请选择身份：" << endl;
        cout << "\033[32m                  1. 管理员 " << endl;
        cout << "\033[32m                  2. 住户\n" << endl;
        cout << "\033[33m===================================================\033[0m" << endl;


        cin >> option;
        if (option == 1 && Login("admin", "0")) {

            adminMenu();
        }
        else if (option == 2) {
            string userid;
            cout << "请输入住户id" << endl;
            cin >> userid;
            if (Login("user", userid)) {
                userMenu(userid);
            }
            else {
                continue;
            }
        }
        else {
            cout << "已退出！" << endl;
        }
    } while (option != 0);


    system("pause");
    return 0;
}
