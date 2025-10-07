#include <ctemplate/template.h>
#include <iostream>
#include <string>

int main() {
    ctemplate::TemplateDictionary dict("my_template");
    dict.SetValue("TITLE", "My Page");
    dict.SetValue("USERNAME", "John Doe");

    // 添加ITEMS
    auto item = dict.AddSectionDictionary("ITEMS");
    item->SetValue("ITEM_NAME", "Laptop");

    // 如果没有ITEMS，就显示NO_ITEMS section
    // 如果要测试空状态，注释掉上面的ITEMS添加代码，并取消下面的注释：
    // dict.ShowSection("NO_ITEMS");

    std::string output;
    if (!ctemplate::ExpandTemplate("template.tpl", ctemplate::DO_NOT_STRIP, &dict, &output)) {
        std::cerr << "ExpandTemplate failed: cannot load template.tpl\n";
        return 1;
    }
    std::cout << output << std::endl;
    return 0;
}