async function loadProducts() {

    const response = await fetch("/api/products");

    const data = await response.json();

    const table = document.getElementById("productTable");

    table.innerHTML = "";

    let totalStock = 0;

    data.products.forEach(function(product) {

        totalStock += product.quantity;

        const row = `
            <tr>
                <td>${product.id}</td>
                <td>${product.name}</td>
                <td>${product.category}</td>
                <td>${product.quantity}</td>
                <td>Rs. ${product.price}</td>
            </tr>
        `;

        table.innerHTML += row;
    });

    document.getElementById("totalProducts").innerText =
        data.products.length;

    document.getElementById("totalStock").innerText =
        totalStock;

    document.getElementById("totalOrders").innerText =
        data.orders;
}


async function searchProduct() {

    const id = document.getElementById("searchId").value;

    const response =
        await fetch("/api/search?id=" + id);

    const product = await response.json();

    const result = document.getElementById("searchResult");

    if (product.found) {

        result.innerHTML =
            "Product: " + product.name +
            "<br>Category: " + product.category +
            "<br>Quantity: " + product.quantity +
            "<br>Price: Rs. " + product.price;

    } else {

        result.innerHTML = "Product not found.";
    }
}


async function updateQuantity() {

    const id =
        document.getElementById("updateId").value;

    const quantity =
        document.getElementById("newQuantity").value;

    const response = await fetch(
        "/api/update?id=" + id + "&quantity=" + quantity
    );

    const result = await response.json();

    alert(result.message);

    loadProducts();
}


async function placeOrder() {

    const productId =
        document.getElementById("orderProductId").value;

    const quantity =
        document.getElementById("orderQuantity").value;

    const response = await fetch(
        "/api/order?productId=" +
        productId +
        "&quantity=" +
        quantity
    );

    const result = await response.json();

    document.getElementById("orderMessage").innerText =
        result.message;

    loadProducts();
}


async function trackOrder() {

    const id =
        document.getElementById("orderId").value;

    const response =
        await fetch("/api/track?id=" + id);

    const result = await response.json();

    const box =
        document.getElementById("orderResult");

    if (result.found) {

        box.innerHTML =
            "Order ID: " + result.orderId +
            "<br>Product ID: " + result.productId +
            "<br>Quantity: " + result.quantity +
            "<br>Status: " + result.status;

    } else {

        box.innerHTML = "Order not found.";
    }
}


loadProducts();