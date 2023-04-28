# C++でTypeScriptのZodを実現しようという試み

整数値のバリデーション

```cpp
using int0to64 = zod<int>::min<0>::max<64>;

int main() {
    int0to64 v1 = 10;
    int0to64 v2 = 70; // 64を超えるので実行時エラー
    int0to64 v3 = -10; // 0を下回るので実行時エラー

    std::cout << v1.value << std::endl;
    std::cout << v2.value << std::endl;
}
```

文字列のバリデーション
```cpp
using string10 = zod<std::string>::max<10>;
using email = zod<std::string>::email;
using url = zod<std::string>::url;
using word10 = zod<std::string>::regex<r(R"(\w+)")>::max<10>;

int main() {
    string10 s1 = "100";
    string10 s2 = "01234567890"; // 10文字以上なので実行時エラー
    email e1 = "test@example.com";
    email e2 = "testexample.com"; // メールアドレスのフォーマットに合わないので実行時エラー
    url u1 = "http://github.com";
    url u2 = "http://github.com a"; // URLのフォーマットに合わないので実行時エラー
    word10 w1 = "perfect";
    word10 w2 = "perfect_human"; // 10文字異常なので実行時エラー
    word10 w3 = "perf human"; // スペースが含まれているため実行時エラー
}
```