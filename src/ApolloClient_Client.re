module Types = ApolloClient_Types;

type queryObj('raw_t_variables) = {
  query: Types.queryString,
  variables: 'raw_t_variables,
  errorPolicy: Types.errorPolicy,
};

type opaqueQueryObj;
external toOpaqueQueryObj: queryObj('raw_t_variables) => opaqueQueryObj =
  "%identity";

type mutationObj('raw_t_variables) = {
  mutation: Types.queryString,
  variables: 'raw_t_variables,
};

type updateQueryOptions('raw_t, 'raw_t_variables) = {
  fetchMoreResult: option('raw_t),
  variables: option('raw_t_variables),
};

type onErrorT;
type updateQueryT('raw_t, 'raw_t_variables) =
  ('raw_t, updateQueryOptions('raw_t, 'raw_t_variables)) => 'raw_t;

type updateSubscriptionOptions = {
  subscriptionData: option(Js.Json.t),
  variables: option(Js.Json.t),
};
type updateQuerySubscriptionT =
  (Js.Json.t, updateSubscriptionOptions) => Js.Json.t;

type subscribeToMoreOptions = {
  document: Types.queryString,
  variables: option(Js.Json.t),
  updateQuery: option(updateQuerySubscriptionT),
  onError: option(onErrorT),
};

type fetchMoreOptions('raw_t, 'raw_t_variables) = {
  variables: option('raw_t_variables),
  updateQuery: option(updateQueryT('raw_t, 'raw_t_variables)),
};

type queryResult('raw_t, 'raw_t_variables) = {
  loading: bool,
  data: Js.Nullable.t('raw_t),
  error: Js.Nullable.t(Types.apolloError),
  refetch:
    Js.Null_undefined.t('raw_t) =>
    Js.Promise.t(queryResult('raw_t, 'raw_t_variables)),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
  fetchMore:
    fetchMoreOptions('raw_t, 'raw_t_variables) => Js.Promise.t(unit),
  subscribeToMore: subscribeToMoreOptions => unit,
};

type mutationResult('raw_t) = {
  loading: bool,
  called: bool,
  data: Js.Nullable.t('raw_t),
  error: Js.Nullable.t(Types.apolloError),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
};

type t;

[@bs.send]
external query:
  (t, queryObj('raw_t_variables)) =>
  Js.Promise.t(queryResult('raw_t, 'raw_t_variables)) =
  "query";

[@bs.send]
external mutate:
  (t, mutationObj('raw_t_variables)) => Js.Promise.t(mutationResult('raw_t)) =
  "mutate";
[@bs.send] external resetStore: t => Js.Promise.t(unit) = "resetStore";

type options = {
  link: Types.apolloLink,
  cache: Types.apolloCache,
  ssrMode: option(bool),
  ssrForceFetchDelay: option(int),
  connectToDevTools: option(bool),
  queryDeduplication: option(bool),
};

[@bs.module "@apollo/client"] [@bs.new]
external makeJs: options => t = "ApolloClient";

let make =
    (
      ~link,
      ~cache,
      ~ssrMode=?,
      ~ssrForceFetchDelay=?,
      ~connectToDevTools=?,
      ~queryDeduplication=?,
      (),
    ) => {
  makeJs({
    link,
    cache,
    ssrMode,
    ssrForceFetchDelay,
    connectToDevTools,
    queryDeduplication,
  });
};

[@bs.module "graphql-tag"] external gql: Types.gql = "default";

module ReadQuery = (Operation: Types.Operation) => {
  type readQueryOptions = {
    query: Types.queryString,
    variables: Js.Nullable.t(Operation.Raw.t_variables),
  };
  type response = option(Operation.t);
  [@bs.send]
  external readQuery: (t, readQueryOptions) => Js.Nullable.t(Operation.Raw.t) =
    "readQuery";

  let graphqlQueryAST = gql(. Operation.query);
  let apolloDataToRecord: Js.Nullable.t(Operation.Raw.t) => response =
    apolloData =>
      Js.Nullable.toOption(apolloData)->(Belt.Option.map(Operation.parse));

  let make = (~client, ~variables: option(Operation.t_variables)=?, ()) =>
    readQuery(
      client,
      {
        query: graphqlQueryAST,
        variables:
          Js.Nullable.fromOption(
            switch (variables) {
            | Some(variables) =>
              Some(Operation.serializeVariables(variables))
            | None => None
            },
          ),
      },
    )
    ->apolloDataToRecord;
};

module WriteQuery = (Operation: Types.Operation) => {
  type writeQueryOptions = {
    query: Types.queryString,
    variables: Js.Nullable.t(Operation.Raw.t_variables),
    data: Operation.t,
  };

  [@bs.send]
  external writeQuery: (t, writeQueryOptions) => unit = "writeQuery";

  let graphqlQueryAST = gql(. Operation.query);

  let make =
      (
        ~client,
        ~variables: option(Operation.t_variables)=?,
        ~data: Operation.t,
        (),
      ) =>
    writeQuery(
      client,
      {
        query: graphqlQueryAST,
        variables:
          Js.Nullable.fromOption(
            switch (variables) {
            | Some(variables) =>
              Some(Operation.serializeVariables(variables))
            | None => None
            },
          ),
        data,
      },
    );
};
