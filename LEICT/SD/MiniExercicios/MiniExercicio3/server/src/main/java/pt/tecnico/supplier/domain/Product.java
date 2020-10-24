package pt.tecnico.supplier.domain;

/**
 * Product entity. Only the product quantity is mutable so its get/set methods
 * are synchronized.
 */
public class Product {
	/** Product identifier. */
	private String productId;
	/** Product description. */
	private String description;
	/** Available quantity of product. */
	private volatile int quantity;
	/** Price of product */
	private int price;

	private pt.tecnico.supplier.grpc.Product.score score;

	/** Create a new product */
	public Product(String pid, String description, int quantity, int price, pt.tecnico.supplier.grpc.Product.score score) {
		this.productId = pid;
		this.description = description;
		this.quantity = quantity;
		this.price = price;
		this.score = score;
	}

	public pt.tecnico.supplier.grpc.Product.score getScore() {
		return score;
	}

	public String getId() {
		return productId;
	}

	public String getDescription() {
		return description;
	}

	public int getPrice() {
		return price;
	}

	/** Synchronized locks object before returning quantity */
	public synchronized int getQuantity() {
		return quantity;
	}

	/** Synchronized locks object before setting quantity */
	public synchronized void setQuantity(int quantity) {
		this.quantity = quantity;
	}

	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("Product [productId=").append(productId);
		builder.append(", description=").append(description);
		builder.append(", quantity=").append(quantity);
		builder.append(", price=").append(price);
		builder.append(", score=").append(score);
		builder.append("]");
		return builder.toString();
	}

}
