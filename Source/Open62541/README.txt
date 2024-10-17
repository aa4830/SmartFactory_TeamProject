

# 디폴트 세팅
* dll 파일 세팅
프로젝트\Source\Open62541\Lib\Binaries\Win64 에 있는 파일들(dll, pdb)을 프로젝트\Binaries\Win64 에 복붙
( 프로젝트\Binaries\Win64 에 dll, pdb 파일이 있어야만 인식하기 때문)



# 새 프로젝트에서 open62541 라이브러리 추가 시 세팅
* build.cs 세팅
프로젝트\Source\Open62541\build.cs.text 파일을 프로젝트 build.cs에 복붙



# 패키징 시
실행파일(exe)과 동일한 위치에 dll 폴더 안에 있는 파일들을 추가해야 실행파일이 실행됨