<?php
namespace app\index\controller;
use think\Controller;
use think\Db;

class Index extends Controller
{
    public function index()
    {
        return $this->fetch();
    }

    protected $beforeActionList = [
        'checkLogin' => ['except'=>'index,login'],
        'initData'  =>  ['only'=>'choicecount,detaildata'],
    ];

    protected function checkLogin(){
        if(empty(cookie('username'))){
            $this->error('请先登录','index/index');
            return;
        }
    }

    public function login($accountname,$password){
        $users = Db::table('user')->where('account',$accountname)->find();
        if(empty($users)) $this->error('该用户不存在');
        if($users['password']!=$password) $this->error('密码错误');
        cookie('username',$accountname);
        $this->redirect('index/choiceCount');
    }

    public function logout(){
        cookie('username',null);
        $this->success('注销成功','index/index');
    }

    public function chart(){
        return $this->fetch();
    }

    private $effectMeasure;//有效称重的数据集


    //用于给everyMeasure属性赋值，过滤掉数据量过少的数据
    public function initData(){
        $everyMeasure =
            Db::table('initial_data')
                ->distinct(true)
                ->field('sign')
                ->select();
            $effectMeasure = [];
            foreach ($everyMeasure as $value){
                $tmp = Db::table('initial_data')
                    ->where('sign',$value['sign'])
                    ->select();
                if(count($tmp)>=6)
                    array_push($effectMeasure,$tmp);
            }
        $this->effectMeasure = $effectMeasure;
    }

    //所有数据，按照每一次测量来分类，过滤掉一些数据
    public function choicecount(){
        return count($this->effectMeasure);
    }

    //查询详细的数据图表
    public function detaildata($num){
        
    }

}
