# Spectral Rendering

## MonteCarlo Integration of SPD

SPD -> XYZ Conversion
$$
X = \int_{\Lambda} x(\lambda)\Phi(\lambda)d\lambda \\
Y = \int_{\Lambda} y(\lambda)\Phi(\lambda)d\lambda \\
Z = \int_{\Lambda} z(\lambda)\Phi(\lambda)d\lambda \\
$$

if $\lambda$ is sampled with p.d.f $p(\lambda)$, montecarlo integration of XYZ is

$$
X = \frac{1}{N}\sum_{i=1}^N \frac{x(\lambda_i)\Phi(\lambda_i)}{p(\lambda_i)} \\
Y = \frac{1}{N}\sum_{i=1}^N \frac{y(\lambda_i)\Phi(\lambda_i)}{p(\lambda_i)} \\
Z = \frac{1}{N}\sum_{i=1}^N \frac{z(\lambda_i)\Phi(\lambda_i)}{p(\lambda_i)} \\
$$

$\Phi(\lambda_i)$ is computed by montecarlo integration of the rendering equation. fully explicit form will be

$$
X = \frac{1}{N}\sum_{i=1}^N \frac{x(\lambda_i)\Phi(\boldsymbol{s}_i, \lambda_i)}{p(\lambda_i)p(\boldsymbol{s}_i)}
$$
where $\boldsymbol{s}_i$ denotes $i$-th sample used for montecarlo integration of the rendering equation, and $p(\boldsymbol{s}_i)$ is its p.d.f.