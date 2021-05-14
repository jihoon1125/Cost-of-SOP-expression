# Cost-of-SOP-expression
[Quine-Mccluskey](https://ko.wikipedia.org/wiki/%EC%BD%B0%EC%9D%B8-%EB%A7%A4%ED%81%B4%EB%9F%AC%EC%8A%A4%ED%82%A4_%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98) 알고리즘을 활용하여 논리식을 간소화 하여 최소 트랜지스터 비용을 구하는 프로그램 구현할 수 있다.
* 개발언어 : C++
* input data : input_minterm.txt
* output data: result.txt

## Problem statement
* input_minterm.txt에서 don't care과 minterm 데이터들을 1의 개수에 따라 정렬하여 저장
* Quine Mccluskey method에 따라 1의 개수가 하나 차이날 때 해밍 거리가 1인 배열들을 제외해가면서 Prime implicants 확보
* Essential prime implicants 확보 후 prime implicant 리스트 정리
* 최종적으로 소자별 transistor 개수 계산

## Verification & results
* Essential prime implicant들을 지운 Prime Implicant table의 값들을 출력하여 제대로 논리식이 최소화 되었는지 확인해볼 수 있다. 
![image](https://user-images.githubusercontent.com/67624104/118232773-03804c80-b4cc-11eb-8d75-f4eb9d69424f.png)





* 다음은 결과화면의 예시이다
![image](https://user-images.githubusercontent.com/67624104/118232844-1dba2a80-b4cc-11eb-9abf-30f4d274fb8f.png)






