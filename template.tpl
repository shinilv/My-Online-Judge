<!DOCTYPE html>
<html>
<head>
    <title>{{TITLE}}</title>
</head>
<body>
    <h1>Welcome, {{USERNAME}}!</h1>
    
    {{#ITEMS}}
    <div class="item">
        <span>{{ITEM_NAME}}</span>
    </div>
    {{/ITEMS}}
    
    <!-- 使用不同的空section语法 -->
    {{#NO_ITEMS}}
    <p>No items available.</p>
    {{/NO_ITEMS}}
</body>
</html>