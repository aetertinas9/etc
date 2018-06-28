

wordsChunk = document.getElementsByTagName('body')[0].textContent.replace(/\s{2,}/g, '').replace(/\n/g, '').split(' ');
wordSortTable = {};
for(i = 0; i < wordsChunk.length; i++){
    var current = wordsChunk[i].toLowerCase();
    wordSortTable[current] = wordSortTable[current] == undefined ? 1 : wordSortTable[current]+1;
}
wordSort = [];
for(var name in wordSortTable){
    if(name.length == 1 && name.match(/\W/g))
    {continue;}

    wordSort.push([name, wordSortTable[name]]);
}
wordSort.sort(function(a, b) {return a[1] - b[1]});
wordSort = wordSort.slice(-200);
str = '';
//불용어 처리 the a and in of to
for(var i=0; i<wordSort.length; i++)
{
if(wordSort[i][0]=='the'||wordSort[i][0]=='a'||wordSort[i][0]=='and'||
    wordSort[i][0]=='in'||wordSort[i][0]=='of'||wordSort[i][0]=='to'||
    wordSort[i][0]=='as'||wordSort[i][0]=='is'||wordSort[i][0]=='on'||
    wordSort[i][0]=='for'||wordSort[i][0]=='by'||wordSort[i][0]=='been'||
    wordSort[i][0]=='it'||wordSort[i][0]=='against'||wordSort[i][0]=='will'||
    wordSort[i][0]=='be'||wordSort[i][0]=='from'||wordSort[i][0]=='was'||
    wordSort[i][0]=='with'||wordSort[i][0]=='at'||wordSort[i][0]=='that'||
    wordSort[i][0]=='what'||wordSort[i][0]=='so'||wordSort[i][0]=='have'||
    wordSort[i][0]=='are'||wordSort[i][0]=='an'||wordSort[i][0]=='its'||
    wordSort[i][0]=='has'||wordSort[i][0]=='not'||wordSort[i][0]=='both'||
    wordSort[i][0]=='this'||wordSort[i][0]=='where'||wordSort[i][0]=='all'||
    wordSort[i][0]=='which'||wordSort[i][0]=='much'||wordSort[i][0]=='two')
{
    wordSort[i][1]='';
    wordSort[i][0]='';
}
}
for(var i = 0; i < wordSort.length; i++){
    str += wordSort[i][1] +"\t"+ wordSort[i][0] + "\n";
}
document.write('<div id="result">'+str);
//console.log(str);
