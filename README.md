# Bookstore项目

###### 姜子丰

## 1.账户系统

### 1.1账户类的组成

账户类中存储 用户名，用户密码，权限，用户ID，当前密码，新密码，当前选中的图书。

存储：账户的存储通过块状链表库，通过ID找寻账户所在的数组，在数组中找到ID对应的信息。

接口：

1.查询函数

功能：查询ID及相应信息

2.注册函数

功能：新建账户，并检测块状列表存储的数组长度是否符合要求，并在不符合要求时进行将过长数组分割成两个数组。

（由于需要不断查询数组的大小，初步打算用vector实现?)

3.删除函数：

功能：实现账户的删除。并检测数组大小是否合乎要求，在不合乎要求时实现借元素，和将两个合并的操作。

4.创建函数

功能：创建账户，并可以自定义账户的权限，并完成类似注册账户的对于块状链表的维护。

账户类信息储存在文件“Account”中

### 1.2账户类的指令

首先定义一个queue,记录账户登陆的顺序，新的账户类变量Account_now，记录最后一个登陆的账户（由于允许多个帐户同时处于登录状态；允许同一帐户同时多次登录。输入的指令视为最后登录的帐户操作。退出系统视为登出所有已登录帐户，故需要储存账户登陆的顺序。）

通过拷贝构造，将队列中最后一个账户ID对应的信息赋值给Account_now，并在之后的指令通过Account_now作用于系统。

#### 1.2.1登陆指令

检测操作关键字su，接受ID，查询该账户的权限等级，如果Account_now的权限等级高于正在登陆的账户的权限等级，则直接将该账户写入queue，并更改Account_now的值。如果权限不足，则在Account库中查找存储在password中的密码并接受输入的密码，比对后如果正确，则将该账户写入队列，并更改Account_now的值。

#### 1.2.2注销指令

检测关键字logout，首先检测queue是否为空，若为空，则返回操作失败，若不为空，则将账号从队列中删除，并将删除后队列的最后一个账户的信息赋值给Account_now。并清空ISBN_Select

#### 1.2.3注册指令

检测关键字register，接收ID，password,username。首先在Account_now中查询ID，若能查询到ID，返回操作失败，反之，则调用Account中的注册函数，进行账户的新建

#### 1.2.4修改密码

检测关键字passwd，接受ID，检测ID的权限，如果为7可以不接受currentPassword，否则，接受currentPassword和newPassword，查询该ID，若查询不到，则返回操作失败，否则，将currentPassword存储到Account类中的对应账户的Password和CurrentPassword中

#### 1.2.5创建账户

检测关键字useradd，接受相关信息，若Privilege大于Account_now的权限，返回操作失败。查询该ID，若已经注册，返回操作失败，否则调用创建函数。

#### 1.2.6删除账户

检测关键字delete，接受ID并查询，若账户不存在或者账户在queue中则返回操作失败。

## 2.图书类

### 2.1图书类的组成

图书类中存储 ISBN，BookName，Author，Keyword，Quantity,Price,TotalCost。

存储：类似账户类，存储在块状链表中，ISBN为图书的ID，Keyword为string类的vector。

（由于需要按字典序输出，所以初步打算使用set存储ISBN？）

接口：

1.show函数（无参数）：

功能：从块状链表第一个元素开始，依次读取储存在链表中的图书信息，并输出。

1*show函数（有参数）：

功能：查询有关参数，并按次序输出相关图书

2.buy函数

功能：接收ISBN以及Quantity，更改图书类中的Quantity，并更改日志类中的count，history。

3.select函数

功能：接受ISBN，并保存在变量ISBN_Select中，查询ISBN，若无改ISBN，则创建新图书，初始化ISBN为ISBN_Select。

4.modify函数

功能：接受输入信息，更改图书信息。

5.keyword函数

功能：接受字符串，并以"|"为界限断开字符串，分别存储相关的信息。

图书类信息存储在文件”Book“中。

6.import函数

功能：更改相应图书的Quantity和TotalCost，并更改日志类中的count和history

7.查询函数

功能：通过ISBN查询是否存在对应图书

8.新建图书

功能：新建图书，并进行块状链表的维护

### 2.2图书类的指令

#### 2.2.1检索图书

检测关键字，有附加参数时调用有参数的show函数，否则调用无参数的show函数。

#### 2.2.2购买图书

接受ISBN，Quantity，调用查询函数，若无相应ISBN，或者Quantity非正整数，返回操作失败。

#### 2.2.3修改图书信息

检测ISBN_Select，若非空，接收修改信息类别，如ISBN等关键字，如果为ISBN，判断不能与原ISBN相同，若为keyword，调用key函数，不能出现重复信息，不能出现附加参数内容为空或附加参数重复，否则返回操作失败。

#### 2.2.4选择图书

调用select函数。

#### 2.2.5图书进货

检测ISBN_Select是否为空，不为空时，判断购入数量和交易总额是否为正整数，在为正整数时调用import函数。

## 3.日志类

### 3.1日志类的组成

包括count，和结构体history，history包含index，账户ID，操作名，金额，sum

接口：

1.show_finance函数(无参数)

功能：打印sum

1*show_finance函数（有参数）

功能：从index-count开始打印history中的交易记录

2.log函数

功能：打印history

3.change函数

功能：在购买和进货时，更改history，记录格式为在history[index]中记录下当前账户的ID，记录下操作名（string类型），并记录下金额，更新sum的值。

日志类存储在文件"log"中。

P.S.以上所有指令在调用之前均需比较当前账户权限和所需权限，以决定是否有足够权限调用相关指令。

P.S.感觉有地方有欠考虑，因此后期会有修改。
