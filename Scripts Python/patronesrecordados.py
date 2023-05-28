import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rcParams["font.family"] = "DejaVu Sans"

# leo los datos
data=np.loadtxt("patronesrecordados.dat")
npatronesrec=np.zeros(50)
error=np.zeros(50)

for i in range(50):
    npatronesrec[i]=np.mean(data[:,i]) # numero de patrones recordados de media
    error[i]=np.sqrt(npatronesrec[i]*(1-npatronesrec[i])/len(data[:,i]))


npatrones=np.linspace(1,50,50) # numero de patrones almacenados

ax = plt.subplot()

# configurar ejes
ax.set_ylabel('Patrones recordados', fontname='DejaVu Sans', fontsize='12')
ax.set_xlabel('Patrones almacenados', fontname='DejaVu Sans', fontsize='12')

# crear la grafica#B4045F
ax.errorbar(npatrones, npatronesrec, yerr=error, linestyle='', marker='', markersize=4, color='#B4045F', linewidth=1.0, capsize=2)
ax.plot(npatrones, npatronesrec, linestyle='', marker='.', markersize=4, color='#3A405A', linewidth=1.0)

# Guardar la gráfica
plt.savefig('grafica_patronesrecordados.pdf',dpi=300, bbox_inches = "tight")
