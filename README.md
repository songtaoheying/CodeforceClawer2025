CodeforcesClawer 2025

这个项目是湘潭大学c语言期末大作业
实现功能:一个c语言爬虫从codeforce上爬取对象的比赛信息并通过网页展示

只想看项目源代码可以不用管project文件夹
project文件夹是vs的完整项目文件,下载后可点击文件夹中exe文件,自解压生成完整文件夹然后可以直接用



下面是作业要求(当然我只实现基本功能)


一、	简介
利用codefoces.com网站提供的API，抓取用户信息，对其比赛情况进行分析并可视化展示。

二、	基本功能
使用C语言、curl、 cJSON，eChart制作一个用户年度比赛的总结
1.	读取用户基本信息
2.	读取一年内的比赛列表，过滤出Div1，2，3，4的比赛
3.	读取用户参加比赛的列表，过滤出Div1，2，3，4的比赛
4.	读取用户参加比赛的排名，各题的分数，获得分数，Rating的变化情况
5.	统计和展示用户参加各级比赛的次数，参赛次数的月度分布，出勤率
6.	展示所有参赛的比赛的级别，名称，时间，排名，题目得分情况，rating的变化，头衔的变化
7.	统计全年各级比赛各题的通过准确性，题目分值的分布（比如最低，最高，均值，方差）等信息，并使用合适的图例展示standing

   
三、	选作功能
1.	分月统计各级比赛各题的通过准确性，题目分支的分布（比如最低，最高，均值，方差）等信息，并使用合适的图例展示
2.	统计比赛中通过题目，分析用户通过题目的难度分布，和知识点分布，并使用合适的图例展示


四、	参考资料

Codeforces API
http://codeforces.com/apiHelp
	
curl
https://curl.se/libcurl/

cJSON
https://github.com/DaveGamble/cJSON

HTML
https://www.runoob.com/html/html-tutorial.html
	
CSS
https://www.runoob.com/css/css-tutorial.html
  
Javascript
https://www.runoob.com/js/js-tutorial.html
 
ECharts
https://echarts.apache.org/zh/index.html
