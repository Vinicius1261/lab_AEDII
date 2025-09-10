public class ContaCorrente {
    private String numeroConta;
    private String cpfTitular;
    private double saldo;

    // Construtor
    public ContaCorrente(String numeroConta, String cpfTitular, double saldoInicial) {
        this.numeroConta = numeroConta;
        this.cpfTitular = cpfTitular;
        this.saldo = saldoInicial;
    }

    // Getters
    public String getNumeroConta() {
        return numeroConta;
    }

    public String getCpfTitular() {
        return cpfTitular;
    }

    public double getSaldo() {
        return saldo;
    }

    // Setters (sem setter para saldo)
    public void setNumeroConta(String numeroConta) {
        this.numeroConta = numeroConta;
    }

    public void setCpfTitular(String cpfTitular) {
        this.cpfTitular = cpfTitular;
    }

    // Método para depositar
    public void depositar(double valor) {
        if (valor > 0) {
            saldo += valor;
            System.out.println("Depósito de R$" + valor + " realizado com sucesso.");
        } else {
            System.out.println("Valor de depósito inválido.");
        }
    }

    // Método para sacar com taxa de 0.5%
    public void sacar(double valor) {
        if (valor > 0) {
            double taxa = valor * 0.005;
            double valorTotal = valor + taxa;

            if (saldo >= valorTotal) {
                saldo -= valorTotal;
                System.out.println("Saque de R$" + valor + " realizado com taxa de R$" + taxa + ".");
            } else {
                System.out.println("Saldo insuficiente para realizar o saque.");
            }
        } else {
            System.out.println("Valor de saque inválido.");
        }
    }

    // Método para imprimir os dados da conta
    public void imprimirDados() {
        System.out.println("Número da Conta: " + numeroConta);
        System.out.println("CPF do Titular: " + cpfTitular);
        System.out.println("Saldo Atual: R$" + String.format("%.2f", saldo));
    }
}