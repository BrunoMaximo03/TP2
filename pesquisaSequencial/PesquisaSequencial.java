import java.util.*;
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
        this("", "", "", new String[] { "NaN" }, new String[] { "NaN" }, new String[] { "NaN" },"", 0, "", "",
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

    public void setLista(ArrayList<Show> lista) {
        this.lista = lista;
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
    public ArrayList<Show> getLista() {
        return this.lista;
    }

    public  Show buscaTitle(String titulo) {
        for(int i = 0; i < lista.size(); i++) {
            System.out.println("Antes do if ");
            if(lista.get(i).getTitle().equals(titulo)) {
                return lista.get(i);
            }
        }
        return null;
    }

    /* ================== CLONE ===================== */

    public Show cloneClasse(Show object) {
        return new Show(object); // utiliza o construtor de cópia    
    }

    //Formata print Diretor[]
    // Array de string que vira um só, separado por virgula
    public String formatDirector() {
        if (director.length == 0) {
            return "NaN";
        } else if (director.length == 1) {
            return director[0];
        }
        return String.join(", ",director);
    }

    //Formata print ListenIn[]
    public String formatListenIn() {
        if (listen_in.length == 0) {
            return "NaN";
        }
        return Arrays.toString(listen_in);
    }

    //Formata print Country[]
    // Array de string que vira um só, separado por virgula
    public String formatCountry() {
        if (country.length == 0) {
            return "NaN";
        } else if (country.length == 1) {
            return country[0];
        }
        return String.join(", ",country); 
    }

    //Formata print Cast[]
    public String formatCast() {
        if (cast.length == 0) {
            return Arrays.toString(new String[] {"NaN"});
        }
        return Arrays.toString(cast);
    }

    //Printa tudo
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
    
    //Le o arquivo csv
    public void readCSV(ArrayList<Show> lista) {
        try {

            String caminho = "disneyplus.csv";
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
                /*try {
                    SimpleDateFormat dateFormat = new SimpleDateFormat("MMMM d, yyyy", Locale.US);
                    show.setDate(str[6].isEmpty() ? null : dateFormat.parse(str[6]));
                } catch (Exception e) {
                    show.setDate(null);
                } */

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

    //Metodos para serem ordenados
    public void ordenar() {
        sort(getCast());
        sort(getListenIn());
    }

   /*  public boolean buscarShow_id(String id) {
        return show_Id.equals(id);
    }*/

    public void swap(String[] array, int i, int j) {
        String temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    //Método de ordenação
    public void sort(String[] array) {
        for (int i = 0; i < array.length - 1; i++) {
            int menor = i;
            for (int j = i + 1; j < array.length; j++) {
                if (array[j].compareTo(array[menor]) < 0) {
                    menor = j;
                }
            }
            swap(array, i, menor);
        }
    }
}

public class PesquisaSequencial {

    //Busca pelo Id
    public static Show buscarID(String id, ArrayList<Show> lista) {
        for (int i = 0; i < lista.size(); i++) {
            if (lista.get(i).getShow_Id().equals(id)) {
                return lista.get(i);
            }   
        }
        return null;
    }

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in, "ISO-8859-1");
        Show dados = new Show();
        ArrayList<Show> todos = new ArrayList<>();
        dados.readCSV(todos); //lendo o csv

       /*  ArrayList<Show> recebeDados = dados.getLista(); //recebe as informações de DADOS que contém o CSV
        for(Show show : recebeDados) {
            show.printShowComplete();
        }*/

        ArrayList<Show> inseridos = new ArrayList<>(); //lista de INSERIDOS CRIADA

        String entrada = scanner.nextLine();

        while (!entrada.equals("FIM")) {
            Show espetaculo = buscarID(entrada,todos);
            if (espetaculo != null) {
                inseridos.add(espetaculo);
            }
            entrada = scanner.nextLine();
        }
    
       /*  for (Show s : inseridos) {
            s.printShowComplete();         PRINT DEBUG
        }
        */
        
        // busca por titulos
        
        int comparacoes = 0;
        long inicio = System.currentTimeMillis();
        
        entrada = scanner.nextLine();
        while (!entrada.equals("FIM")) {
            boolean found = false;
            for (Show show : inseridos) {
                comparacoes++;
                if (show.getTitle().equals(entrada)) {
                    found = true;
                    break; 
                }
            }
            System.out.println(found ? "SIM" : "NAO");
        }

        // Agora sim, pode fechar
        scanner.close();

        long fim = System.currentTimeMillis();
        long tempoExecucao = inicio - fim;

        // montando o arquivo

        try {
            java.io.PrintWriter arquivo = new java.io.PrintWriter("matricula_sequencial.txt", "UTF-8");
            arquivo.printf("850847\t%d \t%d \n", tempoExecucao, comparacoes);
            arquivo.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
