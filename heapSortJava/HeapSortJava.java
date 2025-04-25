//import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
//import java.util.Date;
//import java.util.Locale;
import java.util.Scanner;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;

class Show {

    private String show_Id;
    private String type;
    private String title;
    private String[] director;
    private String[] cast;
    private String[] country;
    private String date;
    private Integer release_year;
    private String rating;
    private String duration;
    private String[] listen_in;
    private ArrayList<Show> lista;

    // Construtor padrão com arrays preenchidos
    public Show() {
        this("", "", "", new String[] { "NaN" }, new String[] { "NaN" }, new String[] { "NaN" }, "", 0, "", "",
                new String[] { "NaN" });
    }

    // Construtor de cópia
    public Show(Show objeto) {
        this.show_Id = objeto.show_Id;
        this.type = objeto.type;
        this.title = objeto.title;
        this.director = objeto.director != null ? objeto.director.clone() : null;
        this.cast = objeto.cast != null ? objeto.cast.clone() : null;
        this.country = objeto.country != null ? objeto.country.clone() : null;
        this.date = objeto.date;
        this.release_year = objeto.release_year;
        this.rating = objeto.rating;
        this.duration = objeto.duration;
        this.listen_in = objeto.listen_in != null ? objeto.listen_in.clone() : null;
        this.lista = new ArrayList<Show>();
    }

    public Show(String show_Id, String type, String title, String[] director, String[] cast, String[] country,
            String date, int release_year, String rating, String duration, String[] listen_in) {
        this.show_Id = show_Id;
        this.type = type;
        this.title = title;
        this.director = director;
        this.cast = cast;
        this.country = country;
        this.date = date;
        this.release_year = release_year;
        this.rating = rating;
        this.duration = duration;
        this.listen_in = listen_in;
        this.lista = new ArrayList<Show>();
    }

    /* ===================== MÉTODOS SETS ===================== */

    public void setShow_Id(String show_Id) {
        this.show_Id = show_Id;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setDirector(String[] director) {
        this.director = director;
    }

    public void setCast(String[] cast) {
        this.cast = cast;
    }

    public void setCountry(String[] country) {
        this.country = country;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public void setReleaseYear(int release_year) {
        this.release_year = release_year;
    }

    public void setRating(String rating) {
        this.rating = rating;
    }

    public void setDuration(String duration) {
        this.duration = duration;
    }

    public void setListenIn(String[] listen_in) {
        this.listen_in = listen_in;
    }

    /* ==================== MÉTODOS GETS ==================== */

    public String getShow_Id() {
        return this.show_Id;
    }

    public String getType() {
        return this.type;
    }

    public String getTitle() {
        return this.title;
    }

    public String[] getDirector() {
        return this.director;
    }

    public String[] getCast() {
        return this.cast;
    }

    public String[] getCountry() {
        return this.country;
    }

    public String getDate() {
        return this.date;
    }

    public int getReleaseYear() {
        return this.release_year;
    }

    public String getRating() {
        return this.rating;
    }

    public String getDuration() {
        return this.duration;
    }

    public String[] getListenIn() {
        return this.listen_in;
    }    

    /* ================== CLONE ===================== */

    public Show cloneClasse(Show object) {
        return new Show(object); // utiliza o construtor de cópia
    }

    // Formata print Diretor[]
    // Array de string que vira um só, separado por virgula
    public String formatDirector() {
        if (director.length == 0) {
            return "NaN";
        } else if (director.length == 1) {
            return director[0];
        }
        return String.join(", ", director);
    }

    // Formata print ListenIn[]
    public String formatListenIn() {
        if (listen_in.length == 0) {
            return "NaN";
        }
        return Arrays.toString(listen_in);
    }

    // Formata print Country[]
    // Array de string que vira um só, separado por virgula
    public String formatCountry() {
        if (country.length == 0) {
            return "NaN";
        } else if (country.length == 1) {
            return country[0];
        }
        return String.join(", ", country);
    }

    // Formata print Cast[]
    public String formatCast() {
        if (cast.length == 0) {
            return Arrays.toString(new String[] { "NaN" });
        }
        return Arrays.toString(cast);
    }

    // Printa tudo
    public void printShowComplete() {
        System.out.println(
                "=> " + show_Id + " ## " + title + " ## " + type + " ## "
                        + formatDirector() + " ## "
                        + formatCast() + " ## "
                        + formatCountry() + " ## "
                        + (date == null ? "March 1, 1900" : date)
                        + " ## " + (release_year == 0 ? "NaN" : release_year) + " ## " + rating + " ## " + duration
                        + " ## " + formatListenIn() + " ##");
    }

    // Le o arquivo csv
    public void readCSV(ArrayList<Show> lista) {
        try {

            String caminho = "/tmp/disneyplus.csv";
            BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(caminho), "UTF-8"));
            br.readLine(); // pula o cabeçalho

            String linhaAtual;
            while ((linhaAtual = br.readLine()) != null) {

                StringBuilder campo = new StringBuilder();
                boolean dentroAspas = false;
                String[] str = new String[12];
                int j = 0;

                for (int i = 0; i < linhaAtual.length(); i++) {
                    char c = linhaAtual.charAt(i);
                    if (c == '"') {
                        dentroAspas = !dentroAspas;
                    } else if (c == ',' && !dentroAspas) {
                        str[j++] = campo.toString().trim();
                        campo.setLength(0);
                    } else {
                        campo.append(c);
                    }
                }

                if (j < str.length) {
                    str[j] = campo.toString().trim();
                }

                if (str[0] == null || str[0].isEmpty())
                    continue;

                Show show = new Show();

                show.setShow_Id(str[0]);
                show.setType(str[1]);
                show.setTitle(str[2]);
                show.setDirector((str[3] == null || str[3].isEmpty()) ? new String[0] : str[3].split(", "));
                show.setCast((str[4] == null || str[4].isEmpty()) ? new String[0] : str[4].split(", "));
                show.setCountry((str[5] == null || str[5].isEmpty()) ? new String[0] : str[5].split(", "));
                show.setDate(str[6]);
                /*
                 * try {
                 * SimpleDateFormat dateFormat = new SimpleDateFormat("MMMM d, yyyy",
                 * Locale.US);
                 * show.setDate(str[6].isEmpty() ? null : dateFormat.parse(str[6]));
                 * } catch (Exception e) {
                 * show.setDate(null);
                 * }
                 */

                show.setReleaseYear((str[7] == null || str[7].isEmpty()) ? 0 : Integer.parseInt(str[7]));
                show.setRating(str[8]);
                show.setDuration(str[9]);
                show.setListenIn((str[10] == null || str[10].isEmpty()) ? new String[0] : str[10].split(", "));
                show.ordenar();
                lista.add(show);
            }

            br.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // Metodos para serem ordenados
    public void ordenar() {
        sort(lista);
        sort(lista);
        // sort(getTitle());
    }

    // Método de ordenação
    public void sort(ArrayList<Show> array) {
        for (int i = 1; i < array.size(); i++) {
            Show tmp = array.get(i);
            int j = i - 1;

            while ((j >= 0)
                    && (array.get(j).getType().concat(getTitle()).compareTo(tmp.getType().concat(getTitle())) > 0)) {
                lista.set(j + 1, array.get(j));
                j--;
            }
            lista.set(j + 1, tmp);
        }

        /*
         * public boolean buscarShow_id(String id) {
         * return show_Id.equals(id);
         * }
         */

        /*
         * public void swap(String[] array, int i, int j) {
         * String temp = array[i];
         * array[i] = array[j];
         * array[j] = temp;
         * }
         */

        // Método de ordenação
        /*
         * public void sort(String[] array) {
         * for (int i = 1; i < n; i++) {
         * int tmp = array[i];
         * int j = i - 1;
         * 
         * while ((j >= 0) && (array[j] > tmp)) {
         * array[j + 1] = array[j];
         * j--;
         * }
         * array[j + 1] = tmp;
         * }
         * }
         * }
         */
    }
}

public class HeapSortJava {

    public void heapSort(ArrayList<Show> listaShows, int[] comparacoes, int[] movimentacoes) {
        int tamanho = listaShows.size();
    
        // constrói o heap máximo (heapify a partir do meio até a raiz)
        for (int i = tamanho / 2 - 1; i >= 0; i--) {
            aplicarHeapify(listaShows, tamanho, i, comparacoes, movimentacoes);
        }
    
        // pega os elementos do heap um a um e reconstruir o heap
        for (int fim = tamanho - 1; fim > 0; fim--) {
            Collections.swap(listaShows, 0, fim); // Move o maior para o final
            movimentacoes[0]++;
            aplicarHeapify(listaShows, fim, 0, comparacoes, movimentacoes); // Reorganiza o heap reduzido
        }
    }
    
    // Reorganiza o heap com base no elemento na posição 'indicePai'
    private void aplicarHeapify(ArrayList<Show> listaShows, int tamanhoHeap, int indicePai, int[] comparacoes, int[] movimentacoes) {
        int indiceMaior = indicePai;
        int indiceFilhoEsquerda = 2 * indicePai + 1;
        int indiceFilhoDireita = 2 * indicePai + 2;
    
        // Verifica se o filho da esquerda é maior que o pai
        if (indiceFilhoEsquerda < tamanhoHeap &&
            compararPorDiretorETitulo(listaShows.get(indiceFilhoEsquerda), listaShows.get(indiceMaior), comparacoes) > 0) {
            indiceMaior = indiceFilhoEsquerda;
        }
    
        // Verifica se o filho da direita é maior que o maior até agora
        if (indiceFilhoDireita < tamanhoHeap &&
            compararPorDiretorETitulo(listaShows.get(indiceFilhoDireita), listaShows.get(indiceMaior), comparacoes) > 0) {
            indiceMaior = indiceFilhoDireita;
        }
    
        // Se o maior não for o pai, faz a troca e aplica o heapify novamente recursivamente
        if (indiceMaior != indicePai) {
            Collections.swap(listaShows, indicePai, indiceMaior);
            movimentacoes[0]++;
            aplicarHeapify(listaShows, tamanhoHeap, indiceMaior, comparacoes, movimentacoes);
        }
    }
    
    // Compara dois shows: primeiro por diretor, e em caso de empate, por título
    private int compararPorDiretorETitulo(Show showA, Show showB, int[] comparacoes) {
        comparacoes[0]++;
    
        String diretorA = showA.getDirector().length > 0 ? showA.getDirector()[0] : "NaN";
        String diretorB = showB.getDirector().length > 0 ? showB.getDirector()[0] : "NaN";
    
        int resultadoComparacaoDiretor = diretorA.compareTo(diretorB);
    
        if (resultadoComparacaoDiretor != 0) {
            return resultadoComparacaoDiretor; // Se os diretores forem diferentes
        } else {
            return showA.getTitle().compareTo(showB.getTitle()); // Critério de desempate por título
        }
    }
    
    public static Show buscarID(String id, ArrayList<Show> lista) {
       // System.out.println(lista.size());
        for (int i = 0; i < lista.size(); i++) {
            if (lista.get(i).getShow_Id().trim().equals(id.trim())) {
                return lista.get(i);
            }
        }
        return null;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Show show = new Show();
        ArrayList<Show> todos = new ArrayList<>();
        show.readCSV(todos);  // instancia um Show só pra acessar o método não estático

        
        ArrayList<Show> inseridos = new ArrayList<>();
        String entrada = scanner.nextLine();

        while (!entrada.equals("FIM")) {
            Show espetaculo = buscarID(entrada,todos);
            if (espetaculo != null) {
                inseridos.add(espetaculo);
            }
            entrada = scanner.nextLine();
        }        

        int[] comparacoes = { 0 };
        int[] movimentacoes = { 0 };
        long inicio = System.currentTimeMillis();

        HeapSortJava heap = new HeapSortJava();

        heap.heapSort(inseridos, comparacoes, movimentacoes);

        for (Show s : inseridos) {
            s.printShowComplete();
        }

        long fim = System.currentTimeMillis();
        scanner.close();

        try {
            java.io.PrintWriter arquivo = new java.io.PrintWriter("matricula_insercao.txt", "UTF-8");
            arquivo.printf("850847\t%d\t%d\t%dms\n", comparacoes[0], movimentacoes[0], (fim - inicio));
            arquivo.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
