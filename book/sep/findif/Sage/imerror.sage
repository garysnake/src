a,k=var('a,k')
f(a,k)=exp(-a*k^2)
pad(a,k)=(1 - (a*k^2)/2)/(1 + (a*k^2)/2)
ppad=plot(pad(2/3,k)-f(2/3,k),(k,0,pi),legend_label='a=2/3',thickness=2)+plot(pad(4/3,k)-f(4/3,k),(k,0,pi),legend_label='a=4/3',thickness=2,linestyle='--',color='black')
ppad.set_legend_options(title='Explicit Error')
ppad.save(filename='junk_sage.pdf',frame=True,gridlines=True,axes_labels=['wavenumber',''])