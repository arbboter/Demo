var $table;
//初始化bootstrap-table的内容
function InitMainTable () {
    //记录页面bootstrap-table全局变量$table，方便应用
    var queryUrl = '#'
    $table = $('#table_user_manager').bootstrapTable({
        // url: queryUrl,
        method: 'get',
        toolbar: '#toolbar',
        dataType: "json",
        striped: true,//设置为 true 会有隔行变色效果
        undefinedText: "空",//当数据为 undefined 时显示的字符
        pagination: true, //分页
        sortable: true,                     //是否启用排序
        sortOrder: "asc",                   //排序方式
        sidePagination: "server",           //分页方式：client客户端分页，server服务端分页（*）
        showPaginationSwitch:true,//是否显示 数据条数选择框
        showColumns: "true",//是否显示 内容列下拉框
        showRefresh: true,                  //是否显示刷新按钮
        pageNumber: 1,//如果设置了分页，首页页码
        pageSize: 10,//如果设置了分页，页面数据条数
        pageList: [10, 25, 50, 100],
        paginationPreText: '上一页',//指定分页条中上一页按钮的图标或文字,这里是<
        paginationNextText: '下一页',//指定分页条中下一页按钮的图标或文字,这里是>
        data_local: "zh-US",//表格汉化
        clickToSelect: true, 
        sidePagination: "client",
        //得到查询的参数
        queryParams: function (params) {//自定义参数，这里的参数是传给后台的，我这是是分页用的
            return {//这里的params是table提供的
                cp: params.offset,//从数据库第几条记录开始
                ps: params.limit//找多少条
            };
        },
        idField: "id",//指定主键列
        columns: [
            {
                checkbox: true, // 显示一个勾选框
                align: 'center' // 居中显示
            },
            {
                title: '序号',
                field: 'id',
                align: 'center',
                sortable: true
            },
            {
                title: '用户名',
                field: 'username',
                align: 'center',
                sortable: true
            },
            {
                title: '真实姓名',
                field: 'fullname',
                align: 'center',
                sortable: true
            },
            {
                title: '角色',
                field: 'role',
                align: 'center',
                sortable: true
            },
            {
                title: '邮箱',
                field: 'email',
                align: 'center',
                sortable: true
            },
            {
                title: '学号',
                field: 'sid',
                align: 'center',
                sortable: true
            },
            {
                title: '手机号',
                field: 'phone',
                align: 'center',
                sortable: true
            },
            {
                title: '班级',
                field: 'cid',
                align: 'center',
                sortable: true
            },
            {
                title: '密码',
                field: 'password',
                align: 'center'
            },
            {
                title: '状态',
                field: 'status',
                align: 'center',
                sortable: true,
                formatter: function (value, row, index) {
                    //自定义显示，这三个参数分别是：value该行的属性，row该行记录，index该行下标
                    return row.status == 0 ? "正常" : "非正常";
                }

            },
            {
                title: '注册时间',
                field: 'create_time',
                align: 'center',
            },
            {
                title: '修改时间',
                field: 'update_time',
                align: 'center'
            },
            {
                title: '操作',
                field: 'operate',
                align: 'center',
                formatter: operateFormatter,
                events: operateEvents
            }
        ],
        data: [{ 'id': 1, 'create_time': '2018年6月15日09:43:11' },
        { 'id': 2 },
        { 'id': 3 },
        { 'id': 4 },
        { 'id': 5 },
        { 'id': 6 },
        { 'id': 7 },
        { 'id': 8 },
        { 'id': 9 },
        { 'id': 10 },
        { 'id': 11 },
        { 'id': 12 },
        { 'id': 13 },
        { 'id': 14 }]
    });

}

//指定table表体操作事件
window.operateEvents = {
    'click #cell_view': function(e, value, row, index) {
        alert('You click view icon, row: ' + JSON.stringify(row));
    },
    'click #cell_edit': function(e, value, row, index) {
        alert('You click edit icon, row: ' + JSON.stringify(row));
    },
    'click #cell_dele': function(e, value, row, index) {
        alert('You click remove icon, row: ' + JSON.stringify(row));
    }
};

//操作栏的格式化
function operateFormatter(value, row, index) {
    var id = index;
    var result = "";
    
    result += "<a type='button' id='cell_view' class='btn btn-xs green'  title='查看'><span class='glyphicon glyphicon-search'></span></a>";
    result += "<a type='button' id='cell_edit' class='btn btn-xs blue'  title='编辑'><span class='glyphicon glyphicon-pencil'></span></a>";
    result += "<a type='button' id='cell_dele' class='btn btn-xs red'  title='删除'><span class='glyphicon glyphicon-remove'></span></a>";

    return result;
}

InitMainTable();
$table.bootstrapTable('refresh');