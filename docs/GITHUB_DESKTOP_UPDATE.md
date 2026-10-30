# Update an existing GitHub Desktop checkout (v0.7A + R4.9 docs.2)

The package is a cumulative project, not a patch to a binary and not a replacement for your Git repository history.

## English

1. Open **GitHub Desktop** and select the existing MegaBurner v0.7A repository. Commit/stash any unrelated local work first.
2. Use **Repository → Show in Finder / Show in Explorer**. Do **not** delete the local `.git` directory.
3. Extract `MegaBurner_MultiChip_v0_7A_ProfileSafe_GitHub_Ready.zip`. Copy the **contents inside** `MegaBurner_MultiChip_v0_7A_ProfileSafe/` into the repo root, merging folders and allowing file replacement. This includes the hidden `.github` folder. Alternatively extract `MegaBurner_MultiChip_v0_7A_ProfileSafe_GitHub_Overlay.zip` directly into the repository root (it contains only added/changed files, with relative paths).
4. Verify GitHub Desktop's Changes list: `README.md`, `assets/gallery/`, new documentation and `hardware/pcb/R4.9-2L-FINAL/` should appear. Inspect the diff; source firmware, root `index.html`, `web/index.html`, chip configuration and Gerbers must remain unchanged.
5. Run `bash tests/run_all.sh` if Bash, Python 3, g++ and Node are available. Review `docs/README_DOCS2_VALIDATION.md` and `SHA256SUMS.txt`.
6. Create a descriptive local commit, for example `docs: bilingual README, photo gallery and PCB R4.9 documentation`. Press **Push origin** once reviewed. **No automatic remote publication was performed**.

## Español

1. En **GitHub Desktop**, seleccionar el repositorio local existente MegaBurner v0.7A. Confirmar o guardar previamente los cambios ajenos.
2. Abrir **Repository → Show in Finder / Show in Explorer**. **No borrar `.git`**.
3. Extraer el ZIP completo. Copiar el **contenido interno** de `MegaBurner_MultiChip_v0_7A_ProfileSafe/` sobre la raíz del repositorio, fusionando directorios y sustituyendo los archivos coincidentes; incluir `.github`. Como alternativa, extraer el ZIP **Overlay** directamente en la raíz: contiene sólo archivos nuevos o modificados.
4. Inspeccionar la pestaña **Changes** y el diff: `README.md`, `assets/gallery/`, `docs/` y `hardware/pcb/R4.9-2L-FINAL/` deberán aparecer. El firmware, `index.html`, `web/index.html`, configuración de chips y archivos de fabricación permanecen sin cambios.
5. Cuando estén disponibles Bash, Python 3, g++ y Node, ejecutar `bash tests/run_all.sh`. Consultar `docs/README_DOCS2_VALIDATION.md` y `SHA256SUMS.txt`.
6. Crear un commit local de documentación y pulsar **Push origin** tras revisarlo. **No se ha publicado nada automáticamente en GitHub**.

### Safety / Seguridad

This is a documentation integration; the R4.9 shield still needs physical validation. A stock 5 V Mega is unsafe with the R4.9 direct-connected shield. / Es una integración documental; el shield R4.9 sigue pendiente de validación física. No usar un Mega de 5 V estándar.
