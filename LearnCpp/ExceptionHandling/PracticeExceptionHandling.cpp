#include <iostream>
using namespace std;

class InstallerException : public std::runtime_error{
public:
    enum class ErrorType{
        DownloadFailed,
        NoBinaryFound,
        NetworkError,
        INstallFailed,
        ConfigError,
        RegistryError,
        BrandingError,
        SystemInfoError,
        Unknown,
        ClientAlreadyWxists,
        InvalidInput,
        ImportError
    };
    
    explicit InstallerException(const string &comp,
                                const string &message,
                                ErrorType type = ErrorType::Unknown)
    : std::runtime_error("[Component : " + comp + "] " + message),
    m_component(comp),
    m_type(type){}
    
    const string& getComponent() const noexcept {
        return m_component;
    }
    
    ErrorType getType() const noexcept{
        return m_type;
    }
    
private:
    string m_component;
    ErrorType m_type;
};

int main(){
    cout << "Exception Handling Practice" << endl;
    for(auto i = 1; i <= 5; i++){
        try{
            if(i == 3){
                throw InstallerException("Registry", "[Error] Exception occured while writing registry value.", InstallerException::ErrorType::RegistryError);
            }
            cout << "Count : " << i << endl;
        }
        catch (const InstallerException &exception){
            cout << exception.what();
        }
    }
    return 0;
}
