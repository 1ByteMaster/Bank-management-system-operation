function showModule(moduleName) {
    cleanupToggleButton();
    document.querySelectorAll('.module').forEach(module => {
        module.style.display = 'none';
    });

    if (moduleName === 'queue') {
        // 如果是排队系统且未登录，显示登录界面
        if (!currentStaff) {
            const queueModule = document.getElementById('queue-module');
            queueModule.style.display = 'block';
            queueModule.innerHTML = `
                <h2>排队管理系统登录</h2>
                <form class="staff-form" onsubmit="handleQueueLogin(event)">
                    <input type="text" id="staff-number" placeholder="工号" required>
                    <input type="password" id="staff-password" placeholder="密码" required>
                    <button type="submit">登录</button>
                </form>
            `;
            return;
        }
    }

    document.getElementById(`${moduleName}-module`).style.display = 'block';
}

// 职员管理系统功能
function staffFunction(action) {
    const contentArea = document.getElementById('staff-content');

    switch (action) {
        case 'query':
            contentArea.innerHTML = `
                <form class="search-form" onsubmit="searchStaff(event)">
                    <select id="search-type">
                        <option value="number">按工号查找</option>
                        <option value="name">按姓名查找</option>
                    </select>
                    <input type="text" id="search-value" placeholder="请输入搜索内容" required>
                    <button type="submit">查询</button>
                </form>
                <div id="search-results" class="search-results"></div>
            `;
            break;

        case 'add':
            contentArea.innerHTML = `
                <form class="staff-form" onsubmit="addStaff(event)">
                    <input type="text" id="staff-number" placeholder="工号" required>
                    <input type="text" id="staff-name" placeholder="姓名" required>
                    <select id="staff-gender" required>
                        <option value="男">男</option>
                        <option value="女">女</option>
                    </select>
                    <input type="number" id="staff-age" placeholder="年龄" required min="18" max="60">
                    <input type="password" id="staff-password" placeholder="密码(6位数字)" required
                           pattern="[0-9]{6}" title="请输入6位数字密码">
                    <select id="staff-group" required>
                        ${staffData.groups.map(group =>
                `<option value="${group}">${group}</option>`
            ).join('')}
                    </select>
                    <button type="submit">确认入职</button>
                </form>
            `;
            break;

        case 'delete':
            contentArea.innerHTML = `
                <form class="staff-form" onsubmit="deleteStaff(event)">
                    <input type="text" id="staff-number" placeholder="工号" required>
                    <input type="password" id="staff-password" placeholder="密码" required>
                    <button type="submit">确认离职</button>
                </form>
            `;
            break;

        case 'modify':
            contentArea.innerHTML = `
                <form class="staff-form" onsubmit="modifyStaffPassword(event)">
                    <input type="text" id="staff-number" placeholder="工号" required>
                    <input type="password" id="old-password" placeholder="原密码" required>
                    <input type="password" id="new-password" placeholder="新密码(6位数字)" 
                           required pattern="[0-9]{6}" title="请输入6位数字密码">
                    <button type="submit">修改密码</button>
                </form>
            `;
            break;

        case 'display':
            displayAllStaff();
            break;

        case 'save':
            saveStaffToLocalStorage();
            alert('职员信息保存成功！');
            break;
    }
}

// 客户数据结构
const bankData = {
    accounts: JSON.parse(localStorage.getItem('bankAccounts')) || [],
    lastAccountNumber: parseInt(localStorage.getItem('lastAccountNumber')) || 10000
};

// 保存数据到本地存储
function saveToLocalStorage() {
    localStorage.setItem('bankAccounts', JSON.stringify(bankData.accounts));
    localStorage.setItem('lastAccountNumber', bankData.lastAccountNumber.toString());
}

// 修改客户管理系统功能
function customerFunction(action) {
    const contentArea = document.getElementById('customer-content');

    switch (action) {
        case 'query':
            contentArea.innerHTML = `
                <form class="search-form" onsubmit="searchAccount(event)">
                    <select id="search-type">
                        <option value="account">按账号查找</option>
                        <option value="name">按姓名查找</option>
                        <option value="date">按开户日期查找</option>
                    </select>
                    <input type="text" id="search-value" placeholder="请输入搜索内容" required>
                    <button type="submit">查询</button>
                </form>
                <div id="search-results" class="search-results"></div>
            `;
            break;

        case 'open':
            contentArea.innerHTML = `
                <form class="account-form" onsubmit="openAccount(event)">
                    <input type="text" id="name" placeholder="姓名" required>
                    <select id="gender" required>
                        <option value="男">男</option>
                        <option value="女">女</option>
                    </select>
                    <input type="password" id="password" placeholder="密码(6位数字)" required 
                           pattern="[0-9]{6}" title="请输入6位数字密码">
                    <input type="number" id="initial-amount" placeholder="初始存款金额" required min="0">
                    <select id="account-type" required>
                        <option value="活期">活期</option>
                        <option value="定期">定期</option>
                    </select>
                    <button type="submit">确认开户</button>
                </form>
            `;
            break;

        case 'close':
            contentArea.innerHTML = `
                <form class="account-form" onsubmit="closeAccount(event)">
                    <input type="text" id="account-number" placeholder="账号" required>
                    <input type="password" id="password" placeholder="密码" required>
                    <button type="submit">确认销户</button>
                </form>
            `;
            break;

        case 'modify':
            contentArea.innerHTML = `
                <form class="account-form" onsubmit="modifyPassword(event)">
                    <input type="text" id="account-number" placeholder="账号" required>
                    <input type="password" id="old-password" placeholder="原密码" required>
                    <input type="password" id="new-password" placeholder="新密码(6位数字)" 
                           required pattern="[0-9]{6}" title="请输入6位数字密码">
                    <button type="submit">修改密码</button>
                </form>
            `;
            break;

        case 'sort':
            contentArea.innerHTML = `
                <div class="sort-controls">
                    <select id="sort-type">
                        <option value="account">按账号排序</option>
                        <option value="name">按姓名排序</option>
                        <option value="balance">按余额排序</option>
                        <option value="date">按开户日期排序</option>
                    </select>
                    <button onclick="sortAccounts()">排序</button>
                </div>
                <div id="sort-results" class="sort-results"></div>
            `;
            break;

        case 'deposit':
            contentArea.innerHTML = `
                <form class="transaction-form" onsubmit="handleDeposit(event)">
                    <input type="text" id="account-number" placeholder="账号" required>
                    <input type="password" id="password" placeholder="密码" required>
                    <input type="number" id="amount" placeholder="存款金额" required min="0">
                    <button type="submit">确认存款</button>
                </form>
            `;
            break;

        case 'withdraw':
            contentArea.innerHTML = `
                <form class="transaction-form" onsubmit="handleWithdraw(event)">
                    <input type="text" id="account-number" placeholder="账号" required>
                    <input type="password" id="password" placeholder="密码" required>
                    <input type="number" id="amount" placeholder="取款金额" required min="0">
                    <button type="submit">确认取款</button>
                </form>
            `;
            break;

        case 'transfer':
            contentArea.innerHTML = `
                <form class="transaction-form" onsubmit="handleTransfer(event)">
                    <input type="text" id="from-account" placeholder="转出账号" required>
                    <input type="password" id="password" placeholder="密码" required>
                    <input type="text" id="to-account" placeholder="转入账号" required>
                    <input type="number" id="amount" placeholder="转账金额" required min="0">
                    <button type="submit">确认转账</button>
                </form>
            `;
            break;

        case 'interest':
            contentArea.innerHTML = `
                <form class="account-form" onsubmit="calculateInterest(event)">
                    <input type="text" id="account-number" placeholder="账号" required>
                    <div class="date-range">
                        <label>计算区间：</label>
                        <input type="date" id="start-date" required onchange="updateEndDateMin()">
                        <span>至</span>
                        <input type="date" id="end-date" required>
                    </div>
                    <button type="submit">计算利息</button>
                </form>
                <div id="interest-result"></div>
            `;

            // 添加账号输入框的事件监听
            document.getElementById('account-number').addEventListener('input', function () {
                const account = bankData.accounts.find(acc => acc.accountNumber === this.value);
                if (account) {
                    // 设置开始日期的最小值为开户日期
                    const openDate = new Date(account.openDate);
                    const startDateInput = document.getElementById('start-date');
                    const openDateStr = openDate.toISOString().split('T')[0];

                    startDateInput.min = openDateStr;
                    startDateInput.value = openDateStr;  // 设置默认值为开户日期

                    // 更新结束日期的最小值
                    updateEndDateMin();
                }
            });
            break;
    }
}

// 排队管理系统功能
let vipNumber = 1;
let normalNumber = 1;
const vipQueue = [];
const normalQueue = [];

// 添加计时和评分相关的数据构
const serviceData = {
    currentVIP: null,  // 当前正在服务的VIP号码
    currentNormal: null,  // 当前正在服务的普通号码
    vipStartTime: null,  // VIP服务开始时间
    normalStartTime: null,  // 普通服务开始时间
    ratings: [],  // 存储评分记录
    averageRating: 0,  // 平评分
    totalServed: 0  // 服务总人数
};

// 修改排队管理系统功能
function queueFunction(action) {
    switch (action) {
        case 'vip-new':
            vipQueue.push(vipNumber++);
            updateQueueDisplay();
            break;

        case 'normal-new':
            normalQueue.push(normalNumber++);
            updateQueueDisplay();
            break;

        case 'vip-next':
            if (serviceData.currentVIP || serviceData.currentNormal) {
                showServiceAlert(serviceData.currentVIP ? 'VIP' : '普通');
                return;
            }
            if (vipQueue.length > 0) {
                const number = vipQueue[0];
                serviceData.currentVIP = number;
                serviceData.vipStartTime = new Date();
                vipQueue.shift();
                updateQueueDisplay();
                startServiceTimer('vip');
            }
            break;

        case 'normal-next':
            if (serviceData.currentVIP || serviceData.currentNormal) {
                showServiceAlert(serviceData.currentVIP ? 'VIP' : '普通');
                return;
            }
            if (normalQueue.length > 0) {
                const number = normalQueue[0];
                serviceData.currentNormal = number;
                serviceData.normalStartTime = new Date();
                normalQueue.shift();
                updateQueueDisplay();
                startServiceTimer('normal');
            }
            break;
    }
}

// 添加服务计时器
function startServiceTimer(type) {
    const timerDiv = document.getElementById(`${type}-timer`);
    let startTime = type === 'vip' ? serviceData.vipStartTime : serviceData.normalStartTime;

    if (timerDiv) {
        const timer = setInterval(() => {
            const currentTime = new Date();
            const elapsedTime = Math.floor((currentTime - startTime) / 1000); // 转换为秒
            const minutes = Math.floor(elapsedTime / 60);
            const seconds = elapsedTime % 60;
            timerDiv.textContent = `服务时长: ${minutes}分${seconds}秒`;
        }, 1000);

        timerDiv.dataset.timerId = timer;
    }
}

// 结束服务并显示评分界面
function endService(type) {
    const timerDiv = document.getElementById(`${type}-timer`);
    if (!timerDiv || !timerDiv.dataset.timerId) return;

    const timer = parseInt(timerDiv.dataset.timerId);
    clearInterval(timer);

    const currentNumber = type === 'vip' ? serviceData.currentVIP : serviceData.currentNormal;
    if (!currentNumber) return;

    showRatingDialog(type, currentNumber);

    // 更新当前服务状态
    if (type === 'vip') {
        serviceData.currentVIP = null;
        serviceData.vipStartTime = null;
    } else {
        serviceData.currentNormal = null;
        serviceData.normalStartTime = null;
    }

    // 更新显示
    updateQueueDisplay();
}

// 显示评分对话框
function showRatingDialog(type, number) {
    const dialog = document.createElement('div');
    dialog.className = 'rating-dialog';
    dialog.innerHTML = `
        <h3>${type === 'vip' ? 'VIP' : '普通'}服务评分</h3>
        <p>请为本次服务评分：</p>
        <div class="rating-stars" id="rating-stars">
            ${Array(5).fill(0).map((_, i) =>
        `<span class="star" data-rating="${i + 1}">★</span>`
    ).join('')}
        </div>
    `;
    document.body.appendChild(dialog);

    // 添加星级评分的事件监听
    const stars = dialog.querySelectorAll('.star');
    stars.forEach(star => {
        star.addEventListener('mouseover', function () {
            const rating = this.dataset.rating;
            stars.forEach(s => {
                if (s.dataset.rating <= rating) {
                    s.classList.add('active');
                } else {
                    s.classList.remove('active');
                }
            });
        });

        star.addEventListener('mouseout', function () {
            stars.forEach(s => s.classList.remove('active'));
        });

        star.addEventListener('click', function () {
            const rating = parseInt(this.dataset.rating);
            submitRating(rating, type, number);
        });
    });
}

// 提交评分
function submitRating(rating, type, number) {
    serviceData.ratings.push({
        type,
        number,
        rating,
        time: new Date(),
        staffNumber: currentStaff.number,
        staffName: currentStaff.name
    });

    // 更新平均评分
    const totalRating = serviceData.ratings.reduce((sum, r) => sum + r.rating, 0);
    serviceData.totalServed = serviceData.ratings.length;
    serviceData.averageRating = totalRating / serviceData.totalServed;

    // 更新职员评分
    const staffs = JSON.parse(localStorage.getItem('staffMembers')) || [];
    const staffIndex = staffs.findIndex(s => s.number === currentStaff.number);
    if (staffIndex !== -1) {
        staffs[staffIndex].servedCount = (staffs[staffIndex].servedCount || 0) + 1;
        const staffRatings = serviceData.ratings.filter(r => r.staffNumber === currentStaff.number);
        staffs[staffIndex].grade = staffRatings.reduce((sum, r) => sum + r.rating, 0) / staffRatings.length;
        localStorage.setItem('staffMembers', JSON.stringify(staffs));
    }

    // 更新显示
    updateServiceStats();
    updateServiceHistory();

    // 移除评分对话框
    document.querySelector('.rating-dialog').remove();
}

// 更新服务统计信息
function updateServiceStats() {
    const statsDiv = document.getElementById('service-stats');
    if (statsDiv) {
        // 计算VIP和普通客户的单独统计
        const vipRatings = serviceData.ratings.filter(r => r.type === 'vip');
        const normalRatings = serviceData.ratings.filter(r => r.type === 'normal');

        const vipAvg = vipRatings.length > 0
            ? vipRatings.reduce((sum, r) => sum + r.rating, 0) / vipRatings.length
            : 0;
        const normalAvg = normalRatings.length > 0
            ? normalRatings.reduce((sum, r) => sum + r.rating, 0) / normalRatings.length
            : 0;

        statsDiv.innerHTML = `
            <div class="stats-card">
                <h4>服务统计</h4>
                <p>总服务人数：${serviceData.totalServed}</p>
                <p>总体平均评分：${serviceData.averageRating.toFixed(1)}⭐</p>
                <p>VIP服务人数：${vipRatings.length} (平均${vipAvg.toFixed(1)}⭐)</p>
                <p>普通服务人数：${normalRatings.length} (平均${normalAvg.toFixed(1)}⭐)</p>
            </div>
        `;
    }
}

// 修改更新服务历史记录显示函数
function updateServiceHistory() {
    const historyDiv = document.getElementById('service-history');
    if (historyDiv) {
        const recentServices = serviceData.ratings.slice(-5).reverse(); // 显示最近5条记录
        historyDiv.innerHTML = `
            <div class="history-card">
                <div class="card-header">
                    <h4>最近服务记录</h4>
                    <button class="collapse-btn" onclick="toggleHistory()">收起</button>
                </div>
                <div id="history-content" class="history-content">
                    ${recentServices.map(record => `
                        <div class="service-record">
                            <span>${record.type === 'vip' ? 'VIP-A' : '普通-B'}${record.number}</span>
                            <span>${'★'.repeat(record.rating)}${'☆'.repeat(5 - record.rating)}</span>
                            <span>${new Date(record.time).toLocaleTimeString()}</span>
                        </div>
                    `).join('')}
                </div>
            </div>
        `;
    }
}

// 添加切换历史记录显示/隐藏功能
function toggleHistory() {
    const historyContent = document.getElementById('history-content');
    const btn = document.querySelector('#service-history .collapse-btn');

    if (historyContent.style.display !== 'none') {
        historyContent.style.display = 'none';
        btn.textContent = '展开';
    } else {
        historyContent.style.display = 'block';
        btn.textContent = '收起';
    }
}

// 修改队列显示函数
function updateQueueDisplay() {
    document.getElementById('vip-numbers').innerHTML = `
        <div class="queue-status">
            ${serviceData.currentVIP ?
            `<div class="current-service">
                    正在服务: A${serviceData.currentVIP}
                    <div id="vip-timer" class="timer"></div>
                    <button onclick="endService('vip')">结束服务</button>
                </div>` : ''
        }
            <div class="waiting-numbers">
                ${vipQueue.map(num => `<div class="queue-number">A${num}</div>`).join('')}
            </div>
        </div>
    `;

    document.getElementById('normal-numbers').innerHTML = `
        <div class="queue-status">
            ${serviceData.currentNormal ?
            `<div class="current-service">
                    正在服务: B${serviceData.currentNormal}
                    <div id="normal-timer" class="timer"></div>
                    <button onclick="endService('normal')">结束服务</button>
                </div>` : ''
        }
            <div class="waiting-numbers">
                ${normalQueue.map(num => `<div class="queue-number">B${num}</div>`).join('')}
            </div>
        </div>
    `;
}

// 修改网点数据结构，包含所有校园地点
const branchData = {
    spots: [
        {
            id: 0,
            name: "南门",
            introduction: "学校主出入口，门口有公交站，附近有各类商铺和餐馆",
            distances: [
                { to: 26, distance: 30 },  // 到学景大酒店
                { to: 6, distance: 150 }   // 到图书馆
            ]
        },
        {
            id: 1,
            name: "西门",
            introduction: "靠近经十路的出入口，门口有BRT公交站，交通便利",
            distances: [
                { to: 2, distance: 200 },  // 到1号教学楼
                { to: 27, distance: 150 }  // 到办公楼
            ]
        },
        {
            id: 2,
            name: "1号教学楼",
            introduction: "导员办公室在1461",
            distances: [
                { to: 1, distance: 70 },
                { to: 3, distance: 50 },
                { to: 4, distance: 60 },
                { to: 27, distance: 40 }
            ]
        },
        {
            id: 3,
            name: "2号教学楼",
            introduction: "最常上课的楼 最后一排有插座",
            distances: [
                { to: 2, distance: 50 },
                { to: 4, distance: 20 }
            ]
        },
        {
            id: 4,
            name: "3号实验楼",
            introduction: "上机课就在这 电梯特别挤",
            distances: [
                { to: 2, distance: 60 },
                { to: 3, distance: 20 },
                { to: 14, distance: 100 }
            ]
        },
        {
            id: 5,
            name: "4号教学楼",
            introduction: "办公楼旁边",
            distances: [
                { to: 27, distance: 30 },
                { to: 17, distance: 120 }
            ]
        },
        {
            id: 6,
            name: "图书馆",
            introduction: "学校标志性建筑，地上6层，设有阅览室、自习室等，环境安静优雅",
            distances: [
                { to: 0, distance: 150 },   // 到南门
                { to: 7, distance: 80 }     // 到逸夫楼
            ]
        },
        {
            id: 7,
            name: "逸夫楼",
            introduction: "图书馆近",
            distances: [
                { to: 6, distance: 20 },
                { to: 19, distance: 30 }
            ]
        },
        {
            id: 8,
            name: "气膜馆",
            introduction: "可预约打羽毛球",
            distances: [
                { to: 9, distance: 20 },
                { to: 10, distance: 10 }
            ]
        },
        {
            id: 9,
            name: "篮球场",
            introduction: "可随时打篮球",
            distances: [
                { to: 8, distance: 20 },
                { to: 10, distance: 30 },
                { to: 11, distance: 20 },
                { to: 17, distance: 10 },
                { to: 23, distance: 50 }
            ]
        },
        {
            id: 10,
            name: "排球场",
            introduction: "打排球",
            distances: [
                { to: 8, distance: 10 },
                { to: 9, distance: 30 },
                { to: 16, distance: 130 }
            ]
        },
        {
            id: 11,
            name: "网球场",
            introduction: "打网球",
            distances: [
                { to: 9, distance: 20 }
            ]
        },
        {
            id: 12,
            name: "纳博士地下超市",
            introduction: "超市＋小吃",
            distances: [
                { to: 13, distance: 10 }
            ]
        },
        {
            id: 13,
            name: "711",
            introduction: "便利店",
            distances: [
                { to: 12, distance: 10 },
                { to: 18, distance: 40 },
                { to: 23, distance: 40 }
            ]
        },
        {
            id: 14,
            name: "西苑餐厅",
            introduction: "学校主要餐厅之一，位于教学区附近，提供多种餐饮选择，价格实惠",
            distances: [
                { to: 19, distance: 100 },  // 到菜鸟驿站
                { to: 22, distance: 80 }    // 到1，2号宿舍楼
            ]
        },
        {
            id: 15,
            name: "东苑餐厅",
            introduction: "靠近学生宿舍区的主要餐厅，一楼是大众餐厅，旁边有民族餐厅",
            distances: [
                { to: 18, distance: 120 },  // 到操场
                { to: 24, distance: 100 }   // 到6，7，8宿舍楼
            ]
        },
        {
            id: 16,
            name: "北苑餐厅",
            introduction: "饭 近办公楼",
            distances: [
                { to: 27, distance: 30 },
                { to: 10, distance: 130 }
            ]
        },
        {
            id: 17,
            name: "校医院",
            introduction: "看病",
            distances: [
                { to: 5, distance: 120 },
                { to: 9, distance: 10 }
            ]
        },
        {
            id: 18,
            name: "操场",
            introduction: "标准400米跑道，设有足球场，是体育课和运动会的主要场地",
            distances: [
                { to: 13, distance: 150 },  // 到711
                { to: 15, distance: 120 },  // 东苑餐厅
                { to: 20, distance: 100 }   // 到海岱美术馆
            ]
        },
        {
            id: 19,
            name: "菜鸟驿站",
            introduction: "取快递",
            distances: [
                { to: 7, distance: 30 },
                { to: 14, distance: 30 },
                { to: 22, distance: 10 }
            ]
        },
        {
            id: 20,
            name: "海岱美术馆",
            introduction: "美术馆 没去过",
            distances: [
                { to: 18, distance: 20 }
            ]
        },
        {
            id: 21,
            name: "教工宿舍",
            introduction: "教职工租住地",
            distances: [
                { to: 27, distance: 30 }
            ]
        },
        {
            id: 22,
            name: "1，2号宿舍楼",
            introduction: "离菜鸟驿站站近",
            distances: [
                { to: 14, distance: 10 },
                { to: 19, distance: 10 }
            ]
        },
        {
            id: 23,
            name: "3，5号宿舍楼",
            introduction: "离西苑 菜鸟驿站 2号楼都近",
            distances: [
                { to: 9, distance: 50 },
                { to: 13, distance: 40 }
            ]
        },
        {
            id: 24,
            name: "6，7，8宿舍楼",
            introduction: "东苑餐厅附近",
            distances: [
                { to: 15, distance: 30 },
                { to: 25, distance: 20 }
            ]
        },
        {
            id: 25,
            name: "9，10，11宿舍楼",
            introduction: "离东苑也近 在6，7号宿舍楼前面",
            distances: [
                { to: 24, distance: 20 },
                { to: 26, distance: 10 }
            ]
        },
        {
            id: 26,
            name: "学景大酒店",
            introduction: "酒店 南门门口",
            distances: [
                { to: 0, distance: 20 },
                { to: 25, distance: 10 }
            ]
        },
        {
            id: 27,
            name: "办公楼",
            introduction: "办公楼 离西门近 这个电梯不挤",
            distances: [
                { to: 1, distance: 60 },
                { to: 2, distance: 40 },
                { to: 5, distance: 30 },
                { to: 16, distance: 30 },
                { to: 21, distance: 30 }
            ]
        }
    ]
};

// 构建邻接矩阵
function buildDistanceMatrix() {
    const n = branchData.spots.length;
    const matrix = Array(n).fill().map(() => Array(n).fill(Infinity));

    // 设置对角线为0
    for (let i = 0; i < n; i++) {
        matrix[i][i] = 0;
    }

    // 填充距离数据
    branchData.spots.forEach(spot => {
        spot.distances.forEach(d => {
            matrix[spot.id][d.to] = d.distance;
            matrix[d.to][spot.id] = d.distance; // 无向图，双向设置距离
        });
    });

    return matrix;
}

// 修改网点查询系统功能
function branchFunction(action) {
    const contentArea = document.getElementById('branch-content');

    // 每次调用时先清理之前的展开按钮
    cleanupToggleButton();

    switch (action) {
        case 'search':
            contentArea.innerHTML = `
                <form class="search-form" onsubmit="searchSpot(event)">
                    <select id="spot-search" class="spot-select" required>
                        <option value="">请选择地点</option>
                        ${branchData.spots.map(spot =>
                `<option value="${spot.id}">${spot.name}</option>`
            ).join('')}
                    </select>
                    <button type="submit">查询</button>
                </form>
                <div id="search-results" class="search-results"></div>
            `;
            break;

        case 'route':
            contentArea.innerHTML = `
                <form class="route-form" onsubmit="findRoute(event)">
                    <select id="start-point" required>
                        <option value="">选择起点</option>
                        ${branchData.spots.map(spot =>
                `<option value="${spot.id}">${spot.name}</option>`
            ).join('')}
                    </select>
                    <select id="end-point" required>
                        <option value="">选择终点</option>
                        ${branchData.spots.map(spot =>
                `<option value="${spot.id}">${spot.name}</option>`
            ).join('')}
                    </select>
                    <button type="submit">规划路线</button>
                </form>
                <div id="route-result" class="route-result"></div>
            `;
            break;

        case 'info':
            const spotsPerRow = 4; // 每行显示4个网点
            const initialSpots = branchData.spots.slice(0, spotsPerRow);
            const remainingSpots = branchData.spots.slice(spotsPerRow);

            // 只在显示网点信息时创建展开按钮
            const infoButton = document.querySelector('button[onclick="branchFunction(\'info\')"]');
            if (infoButton) {
                const buttonRect = infoButton.getBoundingClientRect();
                const toggleButton = document.createElement('button');
                toggleButton.id = 'toggle-spots-btn';
                toggleButton.className = 'toggle-spots-btn';
                toggleButton.textContent = '展开全部';
                toggleButton.style.position = 'fixed';
                toggleButton.style.left = `${buttonRect.right + 10}px`;
                toggleButton.style.top = `${buttonRect.top}px`;
                toggleButton.onclick = toggleRemainingSpots;
                document.body.appendChild(toggleButton);
            }

            contentArea.innerHTML = `
                <div class="spots-container">
                    <div class="spot-grid">
                        ${initialSpots.map(spot => `
                            <div class="spot-card">
                                <h3>${spot.name}</h3>
                                <p><strong>简介：</strong>${spot.introduction}</p>
                            </div>
                        `).join('')}
                    </div>
                    <div id="remaining-spots" class="spot-grid" style="display: none;">
                        ${remainingSpots.map(spot => `
                            <div class="spot-card">
                                <h3>${spot.name}</h3>
                                <p><strong>简介：</strong>${spot.introduction}</p>
                            </div>
                        `).join('')}
                    </div>
                </div>
            `;
            break;
    }
}

// 修改搜索地点函数
function searchSpot(event) {
    event.preventDefault();
    const spotId = document.getElementById('spot-search').value;
    const spot = branchData.spots.find(s => s.id === parseInt(spotId));

    const resultsDiv = document.getElementById('search-results');
    if (spot) {
        resultsDiv.innerHTML = `
            <div class="spot-card">
                <h3>${spot.name}</h3>
                <p><strong>简介：</strong>${spot.introduction}</p>
                <p><strong>相邻地点：</strong>${spot.distances.map(d =>
            branchData.spots[d.to].name + `(${d.distance}米)`
        ).join('、')}</p>
            </div>
        `;
    } else {
        resultsDiv.innerHTML = '<p class="no-results">请选择要查询的地点</p>';
    }

}

// 添加显示更多按钮的函数
function showMoreButton(card) {
    const showMoreContainer = document.getElementById('show-more-container');
    const cardRect = card.getBoundingClientRect();

    showMoreContainer.style.display = 'block';
    showMoreContainer.style.position = 'fixed';
    showMoreContainer.style.left = `${cardRect.right + 10}px`;
    showMoreContainer.style.top = `${cardRect.top}px`;
}

// 修改显示更多地点的功能
function toggleRemainingSpots() {
    const remainingSpots = document.getElementById('remaining-spots');
    const toggleBtn = document.getElementById('toggle-spots-btn');

    if (remainingSpots.style.display === 'none') {
        remainingSpots.style.display = 'grid';
        toggleBtn.textContent = '收起列表';
    } else {
        remainingSpots.style.display = 'none';
        toggleBtn.textContent = '展开全部';
    }
}

// 添加清理按钮的功能
function cleanupToggleButton() {
    const toggleBtn = document.getElementById('toggle-spots-btn');
    if (toggleBtn) {
        toggleBtn.remove();
    }
}

// 添加点击其他地方隐藏按钮的功能
document.addEventListener('click', function (event) {
    // 移除之前的点击事件处理逻辑，让按钮保持显示
});

// 查找最短路径（使用Floyd-Warshall算法）
function findShortestPath(matrix, start, end) {
    const n = matrix.length;
    const dist = matrix.map(row => [...row]);
    const next = Array(n).fill().map(() => Array(n).fill(null));

    // 初始化next数组
    for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) {
            if (i !== j && dist[i][j] !== Infinity) {
                next[i][j] = j;
            }
        }
    }

    // Floyd-Warshall算法
    for (let k = 0; k < n; k++) {
        for (let i = 0; i < n; i++) {
            for (let j = 0; j < n; j++) {
                if (dist[i][k] !== Infinity && dist[k][j] !== Infinity) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    // 构建路径
    if (next[start][end] === null) return null;

    const path = [start];
    let current = start;
    while (current !== end) {
        current = next[current][end];
        if (current === null) return null;
        path.push(current);
    }

    return {
        path: path,
        distance: dist[start][end]
    };
}

// 规划路线
function findRoute(event) {
    event.preventDefault();
    const startId = parseInt(document.getElementById('start-point').value);
    const endId = parseInt(document.getElementById('end-point').value);

    if (isNaN(startId) || isNaN(endId)) {
        document.getElementById('route-result').innerHTML =
            '<p class="error">请选择起点和终点</p>';
        return;
    }

    const distanceMatrix = buildDistanceMatrix();
    const result = findShortestPath(distanceMatrix, startId, endId);

    if (result) {
        const pathNames = result.path.map(id => branchData.spots[id].name);
        document.getElementById('route-result').innerHTML = `
            <div class="route-info">
                <h3>推荐路线：</h3>
                <p>${pathNames.join(' → ')}</p>
                <p>总距离：${result.distance} 米</p>
            </div>
        `;
    } else {
        document.getElementById('route-result').innerHTML =
            '<p class="error">无法找到可行路线</p>';
    }
}

// 修改显示服务统计的函数
function showServiceStats() {
    const statsDiv = document.getElementById('service-stats');
    const statsBtn = document.getElementById('stats-btn');

    if (statsDiv.style.display === 'none' || !statsDiv.innerHTML) {
        // 计算统计数据
        const vipRatings = serviceData.ratings.filter(r => r.type === 'vip');
        const normalRatings = serviceData.ratings.filter(r => r.type === 'normal');

        const vipAvg = vipRatings.length > 0
            ? vipRatings.reduce((sum, r) => sum + r.rating, 0) / vipRatings.length
            : 0;
        const normalAvg = normalRatings.length > 0
            ? normalRatings.reduce((sum, r) => sum + r.rating, 0) / normalRatings.length
            : 0;

        // 更新显示
        statsDiv.innerHTML = `
            <div class="stats-card">
                <div class="card-header">
                    <h4>服务统计详情</h4>
                    <button class="collapse-btn" onclick="toggleStats('stats')">收起</button>
                </div>
                <div id="stats-content">
                    <div class="stats-grid">
                        <div class="stats-item">
                            <h5>总体情况</h5>
                            <p>总服务人数：${serviceData.totalServed}</p>
                            <p>总体平均评分：${serviceData.averageRating.toFixed(1)}⭐</p>
                        </div>
                        <div class="stats-item">
                            <h5>VIP服务</h5>
                            <p>服务人数：${vipRatings.length}</p>
                            <p>平均评分：${vipAvg.toFixed(1)}⭐</p>
                        </div>
                        <div class="stats-item">
                            <h5>普通服务</h5>
                            <p>服务人数：${normalRatings.length}</p>
                            <p>平均评分：${normalAvg.toFixed(1)}⭐</p>
                        </div>
                    </div>
                    <div class="stats-chart">
                        <div class="rating-distribution">
                            ${generateRatingDistribution(serviceData.ratings)}
                        </div>
                    </div>
                </div>
            </div>
        `;
        statsDiv.style.display = 'block';
        statsBtn.textContent = '收起服务统计';
    } else {
        statsDiv.style.display = 'none';
        statsBtn.textContent = '查看服务统计';
    }
}

// 生成评分分布图表
function generateRatingDistribution(ratings) {
    const distribution = [0, 0, 0, 0, 0]; // 1-5星的数量统计
    ratings.forEach(r => {
        distribution[r.rating - 1]++;
    });

    const maxCount = Math.max(...distribution);

    return `
        <h5>评分分布</h5>
        <div class="rating-bars">
            ${distribution.map((count, index) => `
                <div class="rating-bar-item">
                    <div class="rating-label">${index + 1}星</div>
                    <div class="rating-bar">
                        <div class="rating-bar-fill" style="width: ${(count / maxCount) * 100}%">
                            ${count}
                        </div>
                    </div>
                </div>
            `).join('')}
        </div>
    `;
}

// 添加切换显示/隐藏功能
function toggleStats(type) {
    const contentDiv = document.getElementById(`${type}-content`);
    const parentDiv = document.getElementById(`${type === 'stats' ? 'service-stats' : 'service-history'}`);
    const btn = parentDiv.querySelector('.collapse-btn');

    if (contentDiv.style.display !== 'none') {
        contentDiv.style.display = 'none';
        btn.textContent = '展开';
    } else {
        contentDiv.style.display = 'block';
        btn.textContent = '收起';
    }
}

// 添加服务提示函数
function showServiceAlert(type) {
    const alertDiv = document.createElement('div');
    alertDiv.className = 'service-alert';
    alertDiv.innerHTML = `
        <div class="alert-content">
            <h4>务提示</h4>
            <p>当前${type}客户正在服务中，请等待当前服务完成后再叫号。</p>
            <button onclick="closeAlert(this)">确定</button>
        </div>
    `;
    document.body.appendChild(alertDiv);

    // 3秒后自动关闭
    setTimeout(() => {
        if (alertDiv.parentNode) {
            alertDiv.parentNode.removeChild(alertDiv);
        }
    }, 3000);
}

// 关闭提示框
function closeAlert(btn) {
    const alertDiv = btn.closest('.service-alert');
    if (alertDiv) {
        alertDiv.parentNode.removeChild(alertDiv);
    }
}

// 页面加载完成后显示默认模块
document.addEventListener('DOMContentLoaded', () => {
    showModule('staff');
});

// 询账户
function searchAccount(event) {
    event.preventDefault();
    const searchType = document.getElementById('search-type').value;
    const searchValue = document.getElementById('search-value').value;
    let results = [];

    switch (searchType) {
        case 'account':
            results = bankData.accounts.filter(acc => acc.accountNumber === searchValue);
            break;
        case 'name':
            results = bankData.accounts.filter(acc => acc.name === searchValue);
            break;
        case 'date':
            results = bankData.accounts.filter(acc => acc.openDate === searchValue);
            break;
    }

    displayAccountResults(results);
}

// 开户
function openAccount(event) {
    event.preventDefault();
    const name = document.getElementById('name').value;
    const gender = document.getElementById('gender').value;
    const password = document.getElementById('password').value;
    const initialAmount = parseFloat(document.getElementById('initial-amount').value);
    const accountType = document.getElementById('account-type').value;

    const account = {
        accountNumber: (++bankData.lastAccountNumber).toString(),
        name,
        gender,
        password,
        balance: initialAmount,
        type: accountType,
        openDate: new Date().toLocaleDateString(),
        status: '正常'
    };

    bankData.accounts.push(account);
    saveToLocalStorage();
    alert(`开户成功！您的账号是：${account.accountNumber}`);
}

// 销户
function closeAccount(event) {
    event.preventDefault();
    const accountNumber = document.getElementById('account-number').value;
    const password = document.getElementById('password').value;

    const accountIndex = bankData.accounts.findIndex(acc =>
        acc.accountNumber === accountNumber && acc.password === password);

    if (accountIndex === -1) {
        alert('账号或密码错误！');
        return;
    }

    bankData.accounts.splice(accountIndex, 1);
    saveToLocalStorage();
    alert('销户成功！');
}

// 修改密码
function modifyPassword(event) {
    event.preventDefault();
    const accountNumber = document.getElementById('account-number').value;
    const oldPassword = document.getElementById('old-password').value;
    const newPassword = document.getElementById('new-password').value;

    const account = bankData.accounts.find(acc =>
        acc.accountNumber === accountNumber && acc.password === oldPassword);

    if (!account) {
        alert('账号或密码错误！');
        return;
    }

    account.password = newPassword;
    saveToLocalStorage();
    alert('密码修改成功！');
}

// 账户排序
function sortAccounts() {
    const sortType = document.getElementById('sort-type').value;
    let sortedAccounts = [...bankData.accounts];

    switch (sortType) {
        case 'account':
            sortedAccounts.sort((a, b) => a.accountNumber.localeCompare(b.accountNumber));
            break;
        case 'name':
            sortedAccounts.sort((a, b) => a.name.localeCompare(b.name));
            break;
        case 'balance':
            sortedAccounts.sort((a, b) => a.balance - b.balance);
            break;
        case 'date':
            sortedAccounts.sort((a, b) => new Date(a.openDate) - new Date(b.openDate));
            break;
    }

    displayAccountResults(sortedAccounts);
}

// 存款
function handleDeposit(event) {
    event.preventDefault();
    const accountNumber = document.getElementById('account-number').value;
    const password = document.getElementById('password').value;
    const amount = parseFloat(document.getElementById('amount').value);

    const account = bankData.accounts.find(acc =>
        acc.accountNumber === accountNumber && acc.password === password);

    if (!account) {
        alert('账号或密码错误！');
        return;
    }

    account.balance += amount;
    saveToLocalStorage();
    alert(`存款成功！当前余额：${account.balance}元`);
}

// 取款
function handleWithdraw(event) {
    event.preventDefault();
    const accountNumber = document.getElementById('account-number').value;
    const password = document.getElementById('password').value;
    const amount = parseFloat(document.getElementById('amount').value);

    const account = bankData.accounts.find(acc =>
        acc.accountNumber === accountNumber && acc.password === password);

    if (!account) {
        alert('账号或密码错误！');
        return;
    }

    if (account.balance < amount) {
        alert('余额不足！');
        return;
    }

    account.balance -= amount;
    saveToLocalStorage();
    alert(`取款成功！当前余额：${account.balance}元`);
}

// 转账
function handleTransfer(event) {
    event.preventDefault();
    const fromAccount = document.getElementById('from-account').value;
    const password = document.getElementById('password').value;
    const toAccount = document.getElementById('to-account').value;
    const amount = parseFloat(document.getElementById('amount').value);

    const sourceAccount = bankData.accounts.find(acc =>
        acc.accountNumber === fromAccount && acc.password === password);
    const targetAccount = bankData.accounts.find(acc =>
        acc.accountNumber === toAccount);

    if (!sourceAccount || !targetAccount) {
        alert('账号或密码错误！');
        return;
    }

    if (sourceAccount.balance < amount) {
        alert('余额不足！');
        return;
    }

    sourceAccount.balance -= amount;
    targetAccount.balance += amount;
    saveToLocalStorage();
    alert(`转账成功！当前余额：${sourceAccount.balance}元`);
}

// 计算利息
function calculateInterest(event) {
    event.preventDefault();
    const accountNumber = document.getElementById('account-number').value;
    const startDate = new Date(document.getElementById('start-date').value);
    const endDate = new Date(document.getElementById('end-date').value);
    const account = bankData.accounts.find(acc => acc.accountNumber === accountNumber);

    if (!account) {
        alert('账号不存在！');
        return;
    }

    if (endDate < startDate) {
        alert('结束日期不能早于开始日期！');
        return;
    }

    // 计算时间差（年）
    const yearDiff = (endDate - startDate) / (1000 * 60 * 60 * 24 * 365);

    // 根据账户类型设置利率
    let interestRate = account.type === '活期' ? 0.0035 : 0.0275;
    const interest = account.balance * interestRate * yearDiff;

    document.getElementById('interest-result').innerHTML = `
        <div class="interest-info">
            <h3>利息计算结果</h3>
            <p>账户类型：${account.type}</p>
            <p>账户余额：${account.balance}元</p>
            <p>计算区间：${startDate.toLocaleDateString()} 至 ${endDate.toLocaleDateString()}</p>
            <p>年利率：${(interestRate * 100).toFixed(2)}%</p>
            <p>计算天数：${Math.floor((endDate - startDate) / (1000 * 60 * 60 * 24))}天</p>
            <p>预计利息：${interest.toFixed(2)}元</p>
        </div>
    `;
}

// 显示账户查询结果
function displayAccountResults(accounts) {
    const resultsDiv = document.getElementById('search-results') ||
        document.getElementById('sort-results');

    if (accounts.length === 0) {
        resultsDiv.innerHTML = '<p class="no-results">未找到相关账户</p>';
        return;
    }

    resultsDiv.innerHTML = `
        <table class="accounts-table">
            <thead>
                <tr>
                    <th>账号</th>
                    <th>姓名</th>
                    <th>性别</th>
                    <th>密码</th>
                    <th>余额</th>
                    <th>账户类型</th>
                    <th>开户日期</th>
                    <th>状态</th>
                </tr>
            </thead>
            <tbody>
                ${accounts.map(acc => `
                    <tr>
                        <td>${acc.accountNumber}</td>
                        <td>${acc.name}</td>
                        <td>${acc.gender}</td>
                        ${createPasswordCell(acc.password)}
                        <td>${acc.balance}</td>
                        <td>${acc.type}</td>
                        <td>${acc.openDate}</td>
                        <td>${acc.status}</td>
                    </tr>
                `).join('')}
            </tbody>
        </table>
    `;
}

// 职员数据结构
const staffData = {
    staffs: JSON.parse(localStorage.getItem('staffMembers')) || [],
    groups: ['GroupA', 'GroupB', 'GroupC']  // 不同的职员组
};

// 保存职员数据到本地存储
function saveStaffToLocalStorage() {
    localStorage.setItem('staffMembers', JSON.stringify(staffData.staffs));
}

// 查询职员
function searchStaff(event) {
    event.preventDefault();
    const searchType = document.getElementById('search-type').value;
    const searchValue = document.getElementById('search-value').value;
    let results = [];

    switch (searchType) {
        case 'number':
            results = staffData.staffs.filter(staff => staff.number === searchValue);
            break;
        case 'name':
            results = staffData.staffs.filter(staff => staff.name === searchValue);
            break;
    }

    displayStaffResults(results);
}

// 添加职员
function addStaff(event) {
    event.preventDefault();
    const number = document.getElementById('staff-number').value;
    const name = document.getElementById('staff-name').value;
    const gender = document.getElementById('staff-gender').value;
    const age = parseInt(document.getElementById('staff-age').value);
    const password = document.getElementById('staff-password').value;
    const group = document.getElementById('staff-group').value;

    // 检查工号是否已存在
    if (staffData.staffs.some(staff => staff.number === number)) {
        alert('该工号已存在！');
        return;
    }

    const staff = {
        number,
        name,
        gender,
        age,
        password,
        group,
        grade: 0,  // 初始评分
        servedCount: 0  // 服务人数
    };

    staffData.staffs.push(staff);
    saveStaffToLocalStorage();
    alert('入职成功！');
}

// 删除职员
function deleteStaff(event) {
    event.preventDefault();
    const number = document.getElementById('staff-number').value;
    const password = document.getElementById('staff-password').value;

    const staffIndex = staffData.staffs.findIndex(staff =>
        staff.number === number && staff.password === password);

    if (staffIndex === -1) {
        alert('工号或密码错误！');
        return;
    }

    staffData.staffs.splice(staffIndex, 1);
    saveStaffToLocalStorage();
    alert('离职办理成功！');
}

// 修改职员密码
function modifyStaffPassword(event) {
    event.preventDefault();
    const number = document.getElementById('staff-number').value;
    const oldPassword = document.getElementById('old-password').value;
    const newPassword = document.getElementById('new-password').value;

    const staff = staffData.staffs.find(staff =>
        staff.number === number && staff.password === oldPassword);

    if (!staff) {
        alert('工号或密码错误！');
        return;
    }

    staff.password = newPassword;
    saveStaffToLocalStorage();
    alert('密码修改成功！');
}

// 显示所有职员
function displayAllStaff() {
    const contentArea = document.getElementById('staff-content');
    displayStaffResults(staffData.staffs, contentArea);
}

// 显示职员查询结果
function displayStaffResults(staffs, container = document.getElementById('search-results')) {
    if (staffs.length === 0) {
        container.innerHTML = '<p class="no-results">未找到相关职员</p>';
        return;
    }

    container.innerHTML = `
        <table class="staff-table">
            <thead>
                <tr>
                    <th>工号</th>
                    <th>姓名</th>
                    <th>性别</th>
                    <th>年龄</th>
                    <th>所属组</th>
                    <th>密码</th>
                    <th>服务评分</th>
                    <th>服务人数</th>
                </tr>
            </thead>
            <tbody>
                ${staffs.map(staff => `
                    <tr>
                        <td>${staff.number}</td>
                        <td>${staff.name}</td>
                        <td>${staff.gender}</td>
                        <td>${staff.age}</td>
                        <td>${staff.group}</td>
                        ${createPasswordCell(staff.password)}
                        <td>${staff.grade.toFixed(1)}</td>
                        <td>${staff.servedCount}</td>
                    </tr>
                `).join('')}
            </tbody>
        </table>
    `;
}

// 添加当前登录职员信息
let currentStaff = null;

// 处理排队系统登录
function handleQueueLogin(event) {
    event.preventDefault();
    const number = document.getElementById('staff-number').value;
    const password = document.getElementById('staff-password').value;

    // 从本地存储中获取职员数据
    const staffs = JSON.parse(localStorage.getItem('staffMembers')) || [];
    const staff = staffs.find(s => s.number === number && s.password === password);

    if (!staff) {
        alert('工号或密码错误！');
        return;
    }

    currentStaff = staff;
    showQueueSystem();
}

// 显示排队系统主界面
function showQueueSystem() {
    const queueModule = document.getElementById('queue-module');
    queueModule.innerHTML = `
        <h2>排队管理系统</h2>
        <div class="staff-info">
            <p>当前操作员：${currentStaff.name} (${currentStaff.number})</p>
            <button onclick="queueLogout()">退出登录</button>
        </div>
        <div class="function-buttons">
            <button onclick="queueFunction('vip-new')">VIP取号</button>
            <button onclick="queueFunction('normal-new')">普通取号</button>
            <button onclick="queueFunction('vip-next')">VIP叫号</button>
            <button onclick="queueFunction('normal-next')">普通叫号</button>
            <button onclick="showServiceStats()">查看服务统计</button>
        </div>
        <div class="queue-display">
            <div class="vip-queue">
                <h3>VIP队列</h3>
                <div id="vip-numbers"></div>
            </div>
            <div class="normal-queue">
                <h3>普通队列</h3>
                <div id="normal-numbers"></div>
            </div>
        </div>
        <div id="service-stats"></div>
        <div id="service-history"></div>
    `;

    // 更新队列显示
    updateQueueDisplay();
}

// 退出登录
function queueLogout() {
    currentStaff = null;
    showModule('queue');
}

// 地图控制相关变量
let scale = 1;
let isDragging = false;
let startX, startY, translateX = 0, translateY = 0;
let currentX = 0, currentY = 0;

// 初始化地图控制
document.addEventListener('DOMContentLoaded', function () {
    const map = document.getElementById('campus-map');
    if (!map) return;

    // 鼠标拖动事件
    map.addEventListener('mousedown', startDragging);
    document.addEventListener('mousemove', drag);
    document.addEventListener('mouseup', stopDragging);

    // 触摸事件支持
    map.addEventListener('touchstart', startDragging);
    document.addEventListener('touchmove', drag);
    document.addEventListener('touchend', stopDragging);

    // 防止图片被拖出
    map.addEventListener('dragstart', (e) => e.preventDefault());
});

// 开始拖动
function startDragging(e) {
    isDragging = true;
    const pos = getPosition(e);
    startX = pos.x - currentX;
    startY = pos.y - currentY;
}

// 拖动中
function drag(e) {
    if (!isDragging) return;
    e.preventDefault();
    const pos = getPosition(e);

    // 降低灵敏度
    const sensitivity = 1.2;  // 降低灵敏度系数
    currentX = (pos.x - startX) * sensitivity;
    currentY = (pos.y - startY) * sensitivity;

    requestAnimationFrame(() => {
        updateMapPosition();
    });
}

// 停止拖动
function stopDragging() {
    isDragging = false;
}

// 获取位置
function getPosition(e) {
    return {
        x: e.type.includes('mouse') ? e.pageX : e.touches[0].pageX,
        y: e.type.includes('mouse') ? e.pageY : e.touches[0].pageY
    };
}

// 更新地图位置时添加平滑过渡
function updateMapPosition() {
    const map = document.getElementById('campus-map');
    map.style.transition = isDragging ? 'none' : 'transform 0.3s ease';
    map.style.transform = `translate(${currentX}px, ${currentY}px) scale(${scale})`;
}

// 放大
function zoomIn() {
    scale = Math.min(scale * 1.3, 4);  // 增加缩放步进值和最大缩放比例
    updateMapPosition();
}

// 缩小
function zoomOut() {
    scale = Math.max(scale / 1.3, 0.3);  // 增加缩放步进值和最小缩放比例
    updateMapPosition();
}

// 重置地图
function resetMap() {
    scale = 1;
    currentX = 0;
    currentY = 0;
    updateMapPosition();
}

// 添加地图展开/收起功能
function toggleMap() {
    const mapContainer = document.getElementById('map-container');
    const toggleBtn = document.getElementById('toggle-map-btn');

    if (mapContainer.style.display !== 'none') {
        mapContainer.style.display = 'none';
        toggleBtn.textContent = '展开地图';
    } else {
        mapContainer.style.display = 'flex';
        toggleBtn.textContent = '收起地图';
        // 重置地图状态
        resetMap();
    }
}

// 页面加载时初始化地图状态
document.addEventListener('DOMContentLoaded', () => {
    const mapContainer = document.getElementById('map-container');
    const toggleBtn = document.getElementById('toggle-map-btn');
    if (mapContainer && toggleBtn) {
        mapContainer.style.display = 'flex';
        toggleBtn.textContent = '收起地图';
    }
});

// 修改更新结束日期最小值的函数
function updateEndDateMin() {
    const startDate = document.getElementById('start-date').value;
    const endDateInput = document.getElementById('end-date');

    if (startDate) {
        // 设置结束日期的最小值为开始日期的后一天
        const minEndDate = new Date(startDate);
        minEndDate.setDate(minEndDate.getDate() + 1);
        endDateInput.min = minEndDate.toISOString().split('T')[0];
    }
}

// 添加密码显示/隐藏功能
function createPasswordCell(password) {
    return `<td>
        <span class="password-text">••••••</span>
        <span class="real-password" style="display:none">${password}</span>
        <button class="toggle-password-btn" onclick="togglePasswordVisibility(this)">
            <span class="show-text">显示</span>
        </button>
    </td>`;
}

// 切换密码显示/隐藏
function togglePasswordVisibility(button) {
    const cell = button.parentElement;
    const passwordText = cell.querySelector('.password-text');
    const realPassword = cell.querySelector('.real-password');
    const showText = button.querySelector('.show-text');

    if (passwordText.style.display !== 'none') {
        passwordText.style.display = 'none';
        realPassword.style.display = 'inline';
        showText.textContent = '隐藏';
    } else {
        passwordText.style.display = 'inline';
        realPassword.style.display = 'none';
        showText.textContent = '显示';
    }
} 