Voici une version complétée et clarifiée de ta synthèse Git :

---

## basics

```bash
git init
```

- Initialise un dépôt Git local.
    

```bash
git clone <url>
```

- Clone le dépôt distant situé à `<url>` dans le répertoire courant.
    

```bash
o/<branch>
```

- Fait référence à la branche `<branch>` sur le dépôt distant (souvent vu avec `origin/<branch>`).
    

---

## Manipulation de branches

> **Note** : `HEAD` désigne le commit sur lequel on se trouve actuellement.

```bash
git branch <name>
```

- Crée une nouvelle branche `<name>` à partir de `HEAD`.
    

```bash
git branch -d <name>
```

- Supprime la branche `<name>` (refuse si la branche n'est pas totalement fusionnée).
    

```bash
git branch -f <name> <commit>
```

- Force la branche `<name>` à pointer vers `<commit>`.
    

---

## Manipulation de commits

```bash
git checkout <branch>/<commit>
```

- Permet de se déplacer sur une branche ou un commit.
    

```bash
git checkout -b <name>
```

- Crée et bascule sur une nouvelle branche `<name>`.
    

```bash
git add <dir>/<file>/.
```

- Ajoute à l’index un fichier, un dossier ou tous les fichiers (`.`).
    

```bash
git commit -m "<message>"
```

- Crée un commit avec les modifications indexées, et y associe le message.
    

```bash
git commit --amend
```

- Modifie le dernier commit (message ou contenu, selon l’index).
    

```bash
git commit -am "<message>"
```

- Ajoute et commit tous les fichiers modifiés suivis (`tracked`), sauf les nouveaux fichiers.
    

```bash
git cherry-pick <C1> <C2> ...
```

- Applique un ou plusieurs commits spécifiques à la branche courante.
    

---

## Commits distants et intégration

```bash
git checkout -b <branch1> o/<branch2>
```

- Crée une branche `<branch1>` locale suivant la branche distante `<branch2>`. (faire `git push origin branch1` va push les modifs de branch1 vers la branche distante de `branch2`).
    

```bash
git merge <branch>
```

- Fusionne la branche `<branch>` dans la branche courante.
    

```bash
git rebase <branch>
```

- Rejoue les commits de la branche courante sur `<branch>` (réécrit l’historique).
    

```bash
git rebase -i <base>
```

- Rebase interactif pour réorganiser, éditer ou fusionner des commits, (permet de sélectionner les commits voulus pour le rebase)

---

## Mise à jour de dépôts

```bash
git push
git push origin <branch>
git push origin <source>:<destination>
```

- Envoie les commits vers le dépôt distant.
    

```bash
git fetch
git fetch origin <branch>
git fetch origin <source>:<destination>
```

- Récupère les nouvelles données du dépôt distant sans fusion.
    

```bash
git pull
git pull origin <branch>
git pull origin <source>:<destination>
```

- Fait un `fetch` suivi d’un `merge` ou `rebase` si l'option `--rebase` est présente.
    

---

## Autres commandes utiles

```bash
git reset --soft | --mixed | --hard <commit>
```

- Déplace `HEAD` vers un commit, et modifie l’index ou le working directory selon l'option.
    

```bash
git tag <name> <commit>
```

- Crée un tag nommé `<name>` sur `<commit>` (ou `HEAD` par défaut).
    

```bash
git revert <commit>
```

- Crée un commit qui annule les effets du commit spécifié.
    

---

## Informations et inspection

```bash
git status
```

- Affiche les fichiers suivis, modifiés, et non suivis, ainsi que la branche courante.
    

```bash
git log --oneline --graph --decorate
```

- Affiche un graphe compact de l’historique des commits.
    

```bash
git diff
```

- Affiche les différences entre le working directory et l’index.
    

```bash
git diff --staged
```

- Affiche les différences entre l’index et le dernier commit.
    

```bash
git reflog
```

- Affiche l’historique des déplacements de `HEAD`.
    

---

## Architecture Git

- **Espace de travail (working space)** : les fichiers de travail visibles.
- **Zone d’indexation (staging area)** : les fichiers prêts à être comités.
- **Dépôt (repository)** : la base de données Git contenant l’historique des commits.
- **origin** : nom du dépôt distant
- **HEAD** : pointeur sur le commit courant
- **HEAD^n** : n-ième parent du commit pointé par `HEAD`.
- **HEAD~n**  : le commit situé n commits avant `HEAD` dans la première lignée de parents (équivalent à suivre `HEAD^` n fois).