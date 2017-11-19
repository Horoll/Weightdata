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

    public function login($accountname,$password){
        $users = Db::table('user')->where('account',$accountname)->find();
        if(empty($users)) $this->error('该用户不存在');
        if($users['password']!=$password) $this->error('密码错误');
        cookie('username',$accountname);
        $this->redirect('index/chart');
    }

    public function logout(){
        cookie('username',null);
        $this->success('注销成功','index/index');
    }

    public function chart(){
        if(empty(cookie('username'))) $this->error('请先登录','index/index');
        return $this->fetch();
    }

    private $everyMeasure;//每次称重的数据集

    protected $beforeActionList = [
        'initDate'  =>  ['only'=>'dataCount,detailData'],
    ];

    //用于给everyMeasure属性赋值
    private function initData(){
        $everyMeasure =
            Db::table('initial_data')
                ->distinct(true)
                ->field('sign')
                ->select();
        $this->everyMeasure = $everyMeasure;
    }

    //所有数据，按照每一次测量来分类
    public function dataCount(){
        return count($this->everyMeasure);
    }


    //查询详细的数据图表
    public function detailData(){

    }

}
