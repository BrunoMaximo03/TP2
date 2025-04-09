import java.io.RandomAccessFile;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;

class Show {

    private String show_Id;
    private String type;
    private String title;
    private String[] director;
    private String[] cast;
    private String[] country;
    private Date date;
    private Integer release_year;
    private String rating;
    private String duration;
    private String[] listen_in;

    public Show() {
        this("", "", "", null, null, null, new Date(), 0, "", "", null);
    }

    public Show(Show objeto) {
    }

    public Show(String show_Id, String type, String title, String[] director, String[] cast, String[] country,
            Date date,
            int release_year, String rating, String duration, String[] listen_in) {
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

    public void setDate(Date date) {
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

    public Date getDate() {
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
        Show objectDupliShow = new Show();

        objectDupliShow.setShow_Id(object.getShow_Id());
        objectDupliShow.setType(object.getType());
        objectDupliShow.setTitle(object.getTitle());
        objectDupliShow.setDirector(object.getDirector());
        objectDupliShow.setCast(object.getCast());
        objectDupliShow.setCountry(object.getCountry());
        objectDupliShow.setDate(object.getDate());
        objectDupliShow.setReleaseYear(object.getReleaseYear());
        objectDupliShow.setRating(object.getRating());
        objectDupliShow.setDuration(object.getDuration());
        objectDupliShow.setListenIn(object.getListenIn());

        return objectDupliShow;
    }

    public void printShowComplete() {
        System.out.println(
                "=> " + show_Id + " ## " + type + " ## " + title + " ## " + Arrays.toString(director) + " ## " + Arrays.toString(cast) +
                        " ## " + Arrays.toString(country) + " ## " + (date == null ? "NaN" : date) + " ## " + (release_year == null ? "NaN" : release_year) + " ## " + rating + " ## " + duration
                        + " ## " + Arrays.toString(listen_in) + "");
    }

    public void read() {

        try {
            RandomAccessFile arqPtr = new RandomAccessFile("tmp/disneyplus.csv", "r");
            arqPtr.seek(0);
            arqPtr.readLine(); //pula o cabeçalho, para não dar erro na Primeira leitura
            String linhaAtual;
            while((linhaAtual = arqPtr.readLine()) != null) { // enquanto nao for fim do arquivo
    
                    StringBuilder campo = new StringBuilder();
                    boolean dentroAspas = false;
    
                    String[] str = new String[12];
                    int j = 0; // Faz referência aos 11 atributos, irá ser o contador deles
    
                    for (int i = 0; i < linhaAtual.length(); i++) {
                        char c = linhaAtual.charAt(i);
                        if (c == '"') {
                            dentroAspas = !dentroAspas;
                        } else if (c == ',' && !dentroAspas) {
                            str[j++] = campo.toString().trim();
                            campo.setLength(0);  // campo fica zerado para ser preenchido novamente
                        } else {
                            campo.append(c);
                        }
                    }
                    
                    if (j < str.length) str[j] = campo.toString().trim();

                    // Proteção: ignora se show_Id (campo obrigatório) estiver vazio ou nulo
                    if (str[0] == null || str[0].isEmpty()) continue;

     
                    this.show_Id = str[0]; //primeira parte do vetor guarda o Show_Id
                    this.type = str[1]; // segunda parte guarda o Type
                    this.title = str[2]; // terceira parte guarda o Title
                    this.director = (str[3] == null || str[3].isEmpty()) ? new String[]{"NaN"} : str[3].split(", "); // se tiver vazio ou null, recebe NaN, se não pega o valor
                    this.cast = (str[4] == null || str[4].isEmpty()) ? new String[]{"NaN"} : str[4].split(", "); // se tiver vazio ou null, recebe NaN, se não pega o valor
                    this.country = (str[5] == null || str[5].isEmpty()) ? new String[]{"NaN"} : str[5].split(", "); // se tiver vazio ou null, recebe NaN, se não pega o valor
                        
                    try {
                        SimpleDateFormat dateFormat = new SimpleDateFormat("MMMM d, yyyy", Locale.US);
                        this.date = str[6].isEmpty() ? null : dateFormat.parse(str[6]); // caso esteja vazio estou tratando no print
                    } catch (Exception e) {
                        System.out.println("Erro ao converter data: " + str[6]);
                        this.date = null;
                    }
    
                    this.release_year = (str[7] == null || str[7].isEmpty()) ? null : Integer.parseInt(str[7]); // caso esteja vazio estou tratando no print
                    this.rating = str[8]; // nona parte guarda o rating
                    this.duration = str[9]; // décima parte guarda o rating
                    this.listen_in = (str[10] == null || str[10].isEmpty()) ? new String[]{"NaN"} : str[10].split(", "); // se tiver vazio ou null, recebe NaN, se não pega o valor
    
                }
                printShowComplete();
    
                // Outros
                arqPtr.close();
           
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public void ordenaAlfabetico(Show[] array) {
        for (int i = 1; i < array.length; i++) {
            Show tmp = array[i];  // pega o elemento atual
            int j = i - 1;
    
            // compara os títulos usando compareTo (ordem alfabética)
            while (j >= 0 && array[j].getTitle().compareToIgnoreCase(tmp.getTitle()) > 0) {
                array[j + 1] = array[j];  // move o elemento para a direita
                j--;
            }
    
            array[j + 1] = tmp;  // insere o elemento na posição correta
        }
    }
    
}

public class Main {

    public static void main(String[] args) {
        Show show = new Show();
        show.read();
    }

}
